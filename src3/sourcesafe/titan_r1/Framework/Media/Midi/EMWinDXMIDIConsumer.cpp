#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMApplication.h" //For listening for quit-messages
#include "EMMediaEngine.h"
#include "EMMediaMIDITrack.h"
#include "EMMediaProject.h"
#include "EMMediaTimer.h"
#include "EMMediaUtility.h"
#include "EMMIDISystem.h"
#include "EMRealtimeMIDIInputDescriptor.h"
#include "EMRealtimeMIDIOutputDescriptor.h"
#include "EMRecordingMIDITrackRepository.h"
#include "EMSemaphore.h"
#include "EMThread.h"
#include "EMWinDXMIDIConsumer.h"
#include "EMWinDXMIDIProducer.h"
#include "EMWinMIDISystem.h"
#include "EMWinRealtimeMIDIInputDescriptor.h"
#include "EMWinRealtimeMIDIOutputDescriptor.h"
#include "EMMediaDebugLog.h"

#include <dmusicc.h>
#include <dmusici.h>
#include <map>
#include <math.h>
#include <objbase.h>
#include <streams.h>

EMWinDXMIDIConsumer* EMWinDXMIDIConsumer::m_opInstance = NULL;

EMWinDXMIDIConsumer* EMWinDXMIDIConsumer::Instance()
{
	if(m_opInstance == NULL)
		m_opInstance = EM_new EMWinDXMIDIConsumer();
	return m_opInstance;
}

void EMWinDXMIDIConsumer::Delete()
{
	delete m_opInstance;
	m_opInstance = NULL;
}

EMWinDXMIDIConsumer::EMWinDXMIDIConsumer()
	:	m_opThread(NULL),
		
		m_opDirectMusicPerformance(NULL),
		m_opIDirectMusicBuffer8(NULL),

		m_opRecordingSemaphore(NULL),
		m_vIsCurrentlyRecording(false),
		m_vShouldRun(true),
		m_vPrfTmStart(0),
		m_vSngTmStart(0),

		m_vIsActiveMIDIInputDevice(false),
		m_vIsInitialized(false),

		m_vLastSongTmEventTime(0),
		m_vLastLengthBytes(0),
		m_vpLastData(NULL),
		m_vNumEvents(0),
		m_vConsumerThreadIsAlive(false)
{
	EMApplication::Instance() -> AddListener(this); //For listening for quit-messages
	m_uShutdownSemaphore = CreateSemaphore(NULL, 0, 1, "WinDXMIDIConsumer shutdown semaphore");
}

EMWinDXMIDIConsumer::~EMWinDXMIDIConsumer()
{
	EMApplication::Instance() -> RemoveListener(this); //Stop listening for quit-messages

	//TODO: Move thread-quit-specific code to MessageReceived!
/*	if(m_vIsCurrentlyRecording)
		StopRecordingE();

	m_vShouldRun = false;
	ReleaseSemaphore(m_uShutdownSemaphore, 1, NULL);

	if(m_opThread != NULL)
	{
		m_opThread -> Kill();
	}
	if(m_opThread != NULL)
	{
		m_opThread -> RemoveListener(this);
		delete m_opThread;
	} */

	for(int16 vEvent = 0; vEvent < m_vNumEvents; vEvent++)
	{
		char vpNumber[10];
		memset(vpNumber, 0, 10);
		itoa(vEvent, vpNumber, 10);
		EMMediaDebugLog::Instance() -> Log(string(string("     Closing down port number ") + string(vpNumber)).c_str());
		m_oPortsMapping[vEvent].m_opPort -> Activate(false);
		m_oPortsMapping[vEvent].m_opPort -> SetReadNotificationHandle(NULL);
		m_oPortsMapping[vEvent].m_opPort -> Release();
		m_oPortsMapping[vEvent].m_opPort = NULL;
		CloseHandle(m_uEvents[vEvent]);
		m_uEvents[vEvent] = NULL;
	}

	//Remove the ports that we failed to activate. If we remove them as soon as the Activate failes, we
	//run the risk of crashing other - working - ports. That's why we stick them into this list and only
	//remove them when shuting down (ie destructing).
	EMMediaDebugLog::Instance() -> Log(" - Removing any failed ports");
	while(m_oFailedPortsToRemove.size() > 0)
	{
		IDirectMusicPort8* opFailedPort = m_oFailedPortsToRemove.front();
		m_oFailedPortsToRemove.pop_front();
		opFailedPort -> Release();
	}

	if(m_opDirectMusicPerformance != NULL)
		m_opDirectMusicPerformance -> CloseDown();

	if(m_opIDirectMusicBuffer8 != NULL)
		m_opIDirectMusicBuffer8 -> Release();

	if(m_opDirectMusicPerformance != NULL)
		m_opDirectMusicPerformance -> Release();

	CloseHandle(m_uShutdownSemaphore);
}

void EMWinDXMIDIConsumer::ClearData()
{
	for(uint16 vIndex = 0; vIndex < 512; vIndex++)
		for(uint16 vIndex2 = 0; vIndex2 < 512; vIndex2++)
			if(m_oPortsMapping[vIndex].m_oppTracks[vIndex2] != NULL)
			{
				m_oPortsMapping[vIndex].m_oppTracks[vIndex2] = NULL;
				m_oPortsMapping[vIndex].m_oppOutputs[vIndex2] = NULL;
			}
}

bool EMWinDXMIDIConsumer::InitCheckE()
{
	if(m_vIsInitialized)
		return true;

	m_opRecordingSemaphore = EMSemaphore::CreateEMSemaphore("EMWinDXMIDIConsumer rec sem", 1, 1);
	HRESULT vResult = S_OK;

	if(m_opDirectMusicPerformance == NULL)
	{
		if(! EMWinMIDISystem::Instance() -> CreatePerformanceE(&m_opDirectMusicPerformance))
			return false;
	}

	DMUS_BUFFERDESC sBufferDescriptor;
	sBufferDescriptor.dwSize = sizeof(DMUS_BUFFERDESC);
	sBufferDescriptor.guidBufferFormat = GUID_NULL;
	sBufferDescriptor.cbBuffer = 512; //Byte size of incoming MIDI event buffer...
	vResult = EMWinMIDISystem::Instance() -> GetDirectMusicObject() -> CreateMusicBuffer(&sBufferDescriptor, &m_opIDirectMusicBuffer8, NULL);

	m_opThread = EMThread::CreateEMThread("EMWinDXMIDIConsumerThread", EM_THREAD_REALTIME_PRIORITY, 0);
	m_opThread -> AddListener(this);
	m_opThread -> StartSuspended();
	m_vIsInitialized = true;
	return true;
}

bool EMWinDXMIDIConsumer::SetTrackAsListener(EMMediaMIDITrack* p_opTrack, EMRealtimeMIDIInputDescriptor* p_opToInput, EMRealtimeMIDIOutputDescriptor* p_opRouteToOutput)
{
	if(p_opToInput != NULL && p_opRouteToOutput != NULL)
	{
		bool m_vShouldLook = true;
		for(uint16 vIndex = 0; vIndex < 512 && m_vShouldLook; vIndex++)
			if(m_oPortsMapping[vIndex].m_opInput != NULL)
				if(m_oPortsMapping[vIndex].m_opInput -> GetID() == p_opToInput -> GetID())
				{
					for(uint16 vIndex2 = 0; vIndex2 < 512; vIndex2++)
					{
						if(m_oPortsMapping[vIndex].m_oppTracks[vIndex2] == p_opTrack)
						{	
							//Track already listening to that input!
							m_vShouldLook = false;
							break;
						}
					}
					for(vIndex2 = 0; vIndex2 < 512 && m_vShouldLook; vIndex2++)
					{
						if(m_oPortsMapping[vIndex].m_oppTracks[vIndex2] == NULL)
						{
							m_oPortsMapping[vIndex].m_oppTracks[vIndex2] = p_opTrack;
							m_oPortsMapping[vIndex].m_oppOutputs[vIndex2] = p_opRouteToOutput;
							m_vShouldLook = false;
							break;
						}
					}
				}
	}
	else 
	{
		for(uint16 vIndex = 0; vIndex < 512; vIndex++)
			for(uint16 vIndex2 = 0; vIndex2 < 512; vIndex2++)
				if(m_oPortsMapping[vIndex].m_oppTracks[vIndex2] == p_opTrack)
				{
					m_oPortsMapping[vIndex].m_oppTracks[vIndex2] = NULL;
					m_oPortsMapping[vIndex].m_oppOutputs[vIndex2] = NULL;
					return true;
				}
	}
	return true;
}

bool EMWinDXMIDIConsumer::AddPortE(EMRealtimeMIDIInputDescriptor* p_opInputDescriptor, void* p_upNativeData)
{
	GUID* upPortGUID = static_cast<GUID*>(p_upNativeData);

	if(! m_vIsInitialized)
		return false;

	HRESULT vResult = S_OK;
	IDirectMusicPort8* opNewPort = NULL;
	HANDLE vIncomingEvent = NULL;

	if(! EMWinMIDISystem::Instance() -> CreatePortE(&opNewPort, *upPortGUID))
		return false;

	vResult = opNewPort -> SetReadNotificationHandle(NULL);
	if(FAILED(vResult))
	{
		opNewPort -> Release();
		return NULL;
	}

	vIncomingEvent = CreateEvent(NULL, false, false, NULL);
	m_uEvents[m_vNumEvents++] = vIncomingEvent;
	
	vResult = opNewPort -> SetReadNotificationHandle(vIncomingEvent);
	if(FAILED(vResult))
	{
		CloseHandle(vIncomingEvent);
		opNewPort -> Release();
		m_vNumEvents--;
		return false;
	}

	m_oPortsMapping[m_vNumEvents - 1].m_uHandle = vIncomingEvent;
	m_oPortsMapping[m_vNumEvents - 1].m_opPort = opNewPort;
	m_oPortsMapping[m_vNumEvents - 1].m_opInput = p_opInputDescriptor;
	vResult = m_opDirectMusicPerformance -> AddPort(opNewPort);
	if(FAILED(vResult))
	{
		m_oPortsMapping[m_vNumEvents - 1].m_uHandle = NULL;
		m_oPortsMapping[m_vNumEvents - 1].m_opPort = NULL;
		m_oPortsMapping[m_vNumEvents - 1].m_opInput = NULL;
		vResult = opNewPort -> SetReadNotificationHandle(NULL);
		opNewPort -> Release();
		CloseHandle(vIncomingEvent);
		m_vNumEvents--;
		return false;
	}

	vResult = m_opDirectMusicPerformance -> AssignPChannelBlock(0, opNewPort, 1);
	if(FAILED(vResult))
	{
		m_oPortsMapping[m_vNumEvents - 1].m_uHandle = NULL;
		m_oPortsMapping[m_vNumEvents - 1].m_opPort = NULL;
		m_oPortsMapping[m_vNumEvents - 1].m_opInput = NULL;
		vResult = opNewPort -> SetReadNotificationHandle(NULL);
		m_oFailedPortsToRemove.push_back(opNewPort);
		CloseHandle(vIncomingEvent);
		m_vNumEvents--;
		return false;
	}
	
	vResult = opNewPort -> Activate(true);
	if(FAILED(vResult))
	{
		m_oPortsMapping[m_vNumEvents - 1].m_uHandle = NULL;
		m_oPortsMapping[m_vNumEvents - 1].m_opPort = NULL;
		m_oPortsMapping[m_vNumEvents - 1].m_opInput = NULL;
		vResult = opNewPort -> SetReadNotificationHandle(NULL);
		m_oFailedPortsToRemove.push_back(opNewPort);
		CloseHandle(vIncomingEvent);
		m_vNumEvents--;
		return false;
	}
	return true;
}

void EMWinDXMIDIConsumer::OnThreadCreated(EMThread* p_opThread)
{
	HRESULT vResult = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(FAILED(vResult))
		EMDebugger("ERROR! Failed to initialize COM system in EMWinDXMIDIConsumer!");
}

void EMWinDXMIDIConsumer::OnThreadKilled(EMThread* p_opThread)
{
	CoUninitialize();
}

bool EMWinDXMIDIConsumer::FilterEvent(uint8* p_vpData, uint8 p_vLength)
{
	//TODO: Implement MIDI event filter here!
	return true;
}

void EMWinDXMIDIConsumer::ThreadRun(EMThread* p_opThread)
{
	if(! m_vShouldRun)
		return;

	REFERENCE_TIME vPrfTmEventTime = (REFERENCE_TIME) 0;
	uint32 vChannelGroup;
	uint32 vLengthBytes;
	BYTE* vpData = NULL;
	REFERENCE_TIME vPrfTmDelta;
	HANDLE uHandles[101];
	int16 vEventNum = 0;

	for(vEventNum = 0; vEventNum < m_vNumEvents; vEventNum++)
		uHandles[vEventNum] = m_oPortsMapping[vEventNum].m_uHandle;
	uHandles[m_vNumEvents] = m_uShutdownSemaphore;

	if(! m_vConsumerThreadIsAlive)
		m_vConsumerThreadIsAlive = true;

	HRESULT vWaitingResult = WaitForMultipleObjects(m_vNumEvents + 1, uHandles, FALSE, INFINITE);
	if(vWaitingResult >= WAIT_OBJECT_0 && vWaitingResult < WAIT_OBJECT_0 + m_vNumEvents)
	{
		vEventNum = vWaitingResult - WAIT_OBJECT_0;
		for(;m_vShouldRun;)
		{
			if(m_oPortsMapping[vEventNum].m_opPort -> Read(m_opIDirectMusicBuffer8) == S_FALSE)
				break;

			m_opIDirectMusicBuffer8 -> ResetReadPtr();
			for(;m_vShouldRun;)
			{
				if(m_opIDirectMusicBuffer8 -> GetNextEvent(&vPrfTmEventTime, &vChannelGroup, &vLengthBytes, &vpData) == S_FALSE)
					break;
				if(vpData[0] != 254 && vpData[0] != 248)
				{
					//Convert NoteOn:s with velocity 0 to NoteOff:s.
					if((vpData[0] & 240) == 144 && vpData[2] == 0)
					{
						vpData[0] = 128; //Note OFF instead of Note ON
						vpData[2] = 0;
					}

					vPrfTmDelta = vPrfTmEventTime - m_vPrfTmStart;

					m_opLastReceiver = m_oPortsMapping[vEventNum].m_opInput;

					m_vLastSongTmEventTime = m_vSngTmStart + (vPrfTmDelta / 10); //Microseconds, Song start 0 = 0
					//m_opRecordingSemaphore -> Release();

					bool vHaveListeners = false;

					for(uint16 vIndex = 0; vIndex < 512; vIndex++)
					{
						EMMediaMIDITrack* opTrack = m_oPortsMapping[vEventNum].m_oppTracks[vIndex];
						if(opTrack != NULL)
						{
							if(opTrack -> IsActive())
							{
								//Filter the event for key-dependent events and apply KO (key offset)
								if(vpData[0] == EM_MIDI_EVENT_TYPE_NOTE_ON || 
									vpData[0] == EM_MIDI_EVENT_TYPE_NOTE_OFF || 
									vpData[0] == EM_MIDI_EVENT_TYPE_POLYPHONIC_AFTERTOUCH)
								{
									int32 vKey = static_cast<int32>(vpData[1]) + opTrack -> GetKeyOffset();
									vpData[1] = (vKey >= -127 && vKey <= 127 ? vKey : (vKey < -127 ? -127 : 127));
								}

								//Filter the event for velocity-dependent events and apply VO (velocity offset)
								if(vpData[0] == EM_MIDI_EVENT_TYPE_NOTE_ON)
								{
									int32 vVelocity = static_cast<int32>(vpData[2]) + opTrack -> GetVelocityOffset();
									vpData[2] = (vVelocity >= -127 && vVelocity <= 127 ? vVelocity : (vVelocity < -127 ? -127 : 127));
								}

								EMWinDXMIDIProducer::Instance() -> SendEvent(m_oPortsMapping[vEventNum].m_oppOutputs[vIndex], m_vLastSongTmEventTime, vLengthBytes, vpData);
							}
							opTrack -> MIDIEventReceived(m_opLastReceiver, m_vLastSongTmEventTime, vLengthBytes, vpData);
							vHaveListeners = true;
						}
					}

					if(! vHaveListeners)
					{
					}
				}
				else
				{
				}
			}
		}
	}
	else if(vWaitingResult == WAIT_OBJECT_0 + m_vNumEvents)
	{
	}
}

void EMWinDXMIDIConsumer::StartE()
{
	if(m_vIsInitialized)
		m_opThread -> Resume();
}

bool EMWinDXMIDIConsumer::StartRecordingE()
{ 
	m_opRecordingSemaphore -> Acquire();
	m_vIsCurrentlyRecording = true;
	m_opDirectMusicPerformance -> GetTime(&m_vPrfTmStart, NULL);
	m_vSngTmStart = EMMediaTimer::Instance() -> NowTime();
	m_opRecordingSemaphore -> Release();
	return true;
}

bool EMWinDXMIDIConsumer::StopRecordingE()
{
	m_opRecordingSemaphore -> Acquire();
	m_vIsCurrentlyRecording = false;
	m_vPrfTmStart = 0;
	m_vSngTmStart = 0;
	m_opRecordingSemaphore -> Release();
	return true;
}


void EMWinDXMIDIConsumer::SetActiveMIDIRoutingInput(bool p_vFlag, int32 p_vOutputID)
{
	if(p_vFlag)
	{
		m_vIsActiveMIDIInputDevice = p_vFlag;
		EMRealtimeOutputDescriptor* opOutput = EMOutputRepository::Instance() -> Find(p_vOutputID);
		if((opOutput -> GetType() & EM_TYPE_MIDI) > 0)
		{
			EMWinRealtimeMIDIOutputDescriptor* opMIDIOutput = static_cast<EMWinRealtimeMIDIOutputDescriptor*>(opOutput);
			EMWinDXMIDIProducer* opProducer = opMIDIOutput -> GetProducer();
			m_opMIDIOutputProducer = opProducer;
		}
	}
	else
	{
		m_vIsActiveMIDIInputDevice = p_vFlag;
	}
}


bool EMWinDXMIDIConsumer::IsActiveMIDIRoutingInput() const
{
	return m_vIsActiveMIDIInputDevice;
}

bool EMWinDXMIDIConsumer::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
		case EM_QUIT_PHASE_ONE:
			return false;
		case EM_QUIT_PHASE_TWO:
			break;
		case EM_QUIT_PHASE_THREE:
		{
			if(m_vIsCurrentlyRecording)
				StopRecordingE();

			m_vShouldRun = false;
			ReleaseSemaphore(m_uShutdownSemaphore, 1, NULL);

			if(m_opThread != NULL)
			{
				m_opThread -> RemoveListener(this);
				m_opThread -> Kill();
			}

			if(m_opThread != NULL)
			{
				delete m_opThread;
				m_opThread = NULL;
			}
			return true;
		}
		case EM_QUIT_PHASE_FOUR:
			break;
		case EM_QUIT_PHASE_FIVE:
			break;
		case EM_QUIT_PHASE_SIX:
			break;
		case EM_QUIT_ABORTED:
			break;
		default:
			break;
	};
	return false;
}

#endif