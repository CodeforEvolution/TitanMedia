#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMApplication.h"
#include "EMListenerRepository.h"
#include "EMMediaClipRepository.h"
#include "EMMediaEngine.h"
#include "EMMediaMIDIEvent.h"
#include "EMMediaMIDIEventPosition.h"
#include "EMMediaMIDIEventRepository.h"
#include "EMMediaMIDITrack.h"
#include "EMMediaProject.h"
#include "EMMediaTimer.h"
#include "EMMediaTrackRepository.h"
#include "EMMediaUtility.h"
#include "EMMIDIClip.h"
#include "EMMIDIClip.h"
#include "EMRealtimeMIDIInputDescriptor.h"
#include "EMRealtimeMIDIOutputDescriptor.h"
#include "EMSemaphore.h"
#include "EMThread.h"
#include "EMWinDXMIDIConsumer.h"
#include "EMWinDXMIDIProducer.h"
#include "EMWinMIDISystem.h"

EMWinDXMIDIProducer* EMWinDXMIDIProducer::m_opInstance = NULL;

#define EM_MIDI_PRODUCER_SHUTDOWN_SEMAPHORE 1

EMWinDXMIDIProducer* EMWinDXMIDIProducer::Instance()
{
	if(m_opInstance == NULL)
		m_opInstance = EM_new EMWinDXMIDIProducer();
	return m_opInstance;
}

void EMWinDXMIDIProducer::Delete()
{  
	delete m_opInstance;
	m_opInstance = NULL;
}

EMWinDXMIDIProducer::EMWinDXMIDIProducer()
	:	m_opThread(NULL),
		m_opDirectMusicPerformance(NULL),
		m_opRunningSemaphore(NULL),
		m_vRefTmStart(0),
		m_vSngTmStart(0),
		m_vActiveRoutingOutputChannel(0),

		m_opEvent(NULL),
		m_vSystemTimeWhenStarted(0),

		m_vIsInitialized(false),

		m_vPChannelCount(0),

		m_opMIDIEventRepository(NULL),
		m_opMediaTimer(NULL),
		m_opActiveInput(NULL),
		m_opActiveOutput(NULL),

		m_uAbortSignal(NULL),

		m_opNextEventToDeliver(NULL),
		m_vNextEventTime(0),
		m_opDirectMusicGraph(NULL)
{
	EMApplication::Instance() -> AddListener(this); //For listening for quit-messages
	m_opRunningSemaphore = EMSemaphore::CreateEMSemaphore(string(string("EMWinDXMIDIProducer prot sem") + EMMediaIDManager::MakeUniqueString()).c_str(), 1, 1);
	m_uAbortSignal = CreateEvent(NULL, FALSE, FALSE, string(string("WinDXMIDIProducer shutdown signal") + EMMediaIDManager::MakeUniqueString()).c_str());
	EMMediaTimer::Instance() -> AddListener(this);
}

EMWinDXMIDIProducer::~EMWinDXMIDIProducer()
{
	EMApplication::Instance() -> RemoveListener(this); //Stop listening for quit-messages

	m_opNextEventToDeliver = NULL;

//	PulseEvent(m_uAbortSignal);				//Moved to MessageReceived EM_QUIT_PHASE_THREE
//	if(m_opThread != NULL)					//Moved to MessageReceived EM_QUIT_PHASE_THREE
//	{										//Moved to MessageReceived EM_QUIT_PHASE_THREE
//		m_opThread -> Kill();				//Moved to MessageReceived EM_QUIT_PHASE_THREE
//		m_opThread -> RemoveListener(this);	//Moved to MessageReceived EM_QUIT_PHASE_THREE
//		delete m_opThread;					//Moved to MessageReceived EM_QUIT_PHASE_THREE
//	}										//Moved to MessageReceived EM_QUIT_PHASE_THREE

	while(m_oPortList.size() > 0)
	{
		IDirectMusicPort8* opPort = m_oPortList.front();
		m_oPortList.pop_front();

		HRESULT vResult = opPort -> Activate(false);
		if(FAILED(vResult))
			eo << "ERROR! Error while deactivating MIDI output port" << ef;
		opPort -> Release();
	}

	EMMediaTimer::Instance() -> RemoveListener(this);

	//Remove the ports that we failed to activate. If we remove them as soon as the Activate failes, we
	//run the risk of crashing other - working - ports. That's why we stick them into this list and only
	//remove them when shuting down (ie destructing).
	while(m_oFailedPortsToRemove.size() > 0)
	{
		IDirectMusicPort8* opFailedPort = m_oFailedPortsToRemove.front();
		m_oFailedPortsToRemove.pop_front();
		m_opDirectMusicPerformance -> RemovePort(opFailedPort);
		opFailedPort -> Release();
	}

	if(m_opDirectMusicGraph != NULL)
		m_opDirectMusicGraph -> Release();
	if(m_opDirectMusicPerformance != NULL)
		m_opDirectMusicPerformance -> CloseDown();
	if(m_opDirectMusicPerformance != NULL)
		m_opDirectMusicPerformance -> Release();
	delete m_opEvent;
	delete m_opRunningSemaphore;

	CloseHandle(m_uAbortSignal);
}

void EMWinDXMIDIProducer::ClearData()
{
	m_opRunningSemaphore -> Acquire();
	m_opMIDIEventRepository = NULL;
	m_opRunningSemaphore -> Release();
}

REFERENCE_TIME EMWinDXMIDIProducer::SongTimeToReferenceTime(int64 p_vSongTime)
{
	REFERENCE_TIME vRefTmEventDeltaTimeFromPlaybackStart = p_vSongTime - m_vSngTmStart;
	REFERENCE_TIME vRefTmTargetTimeOfEvent = m_vRefTmStart + (vRefTmEventDeltaTimeFromPlaybackStart * 10);
	return vRefTmTargetTimeOfEvent;
}

DMUS_PMSG* EMWinDXMIDIProducer::ConvertToDirectMusicMessage(EMMediaMIDIEvent* p_opEvent, IDirectMusicPort8* p_opDestinationPort)
{
	HRESULT vResult = S_OK;
	DMUS_PMSG* upNewMessage = NULL;
	bool vThisEventIsANote = (p_opEvent -> GetBaseEventType() == EM_MIDI_EVENT_TYPE_NOTE_ON || p_opEvent -> GetBaseEventType() == EM_MIDI_EVENT_TYPE_NOTE_OFF);

	//Check if this event comes from the event repository ("off-line" events) or from the MIDI consumer ("on-line" events).
	//This can be checked by looking at the clip id. If it is set, then the event is stored
	//as belonging to some clip in the event repository. If it is -1, then the event comes
	//directly from the MIDI consumer!
	if(p_opEvent -> GetBaseEventType() == EM_MIDI_EVENT_TYPE_NOTE_OFF && p_opEvent -> GetClipID() != -1)
		return NULL;	//Don't handle "off-line" note-off:s. They're handled through the NoteOn's m_opNoteOff-pointer!

    vResult = m_opDirectMusicPerformance -> AllocPMsg((vThisEventIsANote ? sizeof(DMUS_NOTE_PMSG) : sizeof(DMUS_MIDI_PMSG)), reinterpret_cast<DMUS_PMSG**>(&upNewMessage));
	memset(upNewMessage, 0, (vThisEventIsANote ? sizeof(DMUS_NOTE_PMSG) : sizeof(DMUS_MIDI_PMSG)));
	upNewMessage -> dwSize = (vThisEventIsANote ? sizeof(DMUS_NOTE_PMSG) : sizeof(DMUS_MIDI_PMSG));
	upNewMessage -> dwType = (vThisEventIsANote ? DMUS_PMSGT_NOTE : DMUS_PMSGT_MIDI);

	upNewMessage -> dwFlags = DMUS_PMSGF_REFTIME;
	upNewMessage -> rtTime = SongTimeToReferenceTime(*(p_opEvent -> m_vpStart));
	upNewMessage -> mtTime = 0;
	upNewMessage -> dwVirtualTrackID = 0;
	upNewMessage -> pTool = NULL; 
    upNewMessage -> pGraph = NULL;
	upNewMessage -> dwVoiceID = 0;
	upNewMessage -> dwGroupID = 0xFFFFFFFF; 
	upNewMessage -> punkUser = NULL;

	//Set the target MIDI channel (PerformanceChannel, actually) if applicable...
	uint8 vEventType = p_opEvent -> GetBaseEventType();
	if(	vEventType == EM_MIDI_EVENT_TYPE_NOTE_OFF ||
		vEventType == EM_MIDI_EVENT_TYPE_NOTE_ON ||
		vEventType == EM_MIDI_EVENT_TYPE_POLYPHONIC_AFTERTOUCH ||
		vEventType == EM_MIDI_EVENT_TYPE_CONTROL_CHANGE ||
		vEventType == EM_MIDI_EVENT_TYPE_PROGRAM_CHANGE ||
		vEventType == EM_MIDI_EVENT_TYPE_CHANNEL_AFTERTOUCH ||
		vEventType == EM_MIDI_EVENT_TYPE_PITCH_WHEEL_CONTROL)
	{
		//Now, first get the channel base, and then set the MIDI channel for this message...!
		//Also, add 1 to the channel since our channel count is zero-based and DirectMusic's isn't....
		int vPChannelBase = m_oPChannels[p_opDestinationPort];
		upNewMessage -> dwPChannel = vPChannelBase + p_opEvent -> GetPosition() -> GetChannel() +1;
	}
	else
	{
		upNewMessage -> dwPChannel = DMUS_PCHANNEL_BROADCAST_PERFORMANCE;
	}


	//There. We have the DMUS_PMSG or the DMUS_MIDI_PMSG. Now set other cool stuff...
	if(vThisEventIsANote)
	{
		DMUS_NOTE_PMSG* upDirectMusicNoteMessage = reinterpret_cast<DMUS_NOTE_PMSG*>(upNewMessage);
		DMUS_TIMESIGNATURE uTimeSignature;
		MUSIC_TIME uNextInMusicFormat;
		MUSIC_TIME uTimeInMusicFormat;
		m_opDirectMusicPerformance -> ReferenceToMusicTime((*(p_opEvent -> m_vpStart) * 10) + (m_vSystemTimeWhenStarted * 10.0), &uTimeInMusicFormat);
		HRESULT vResult = m_opDirectMusicPerformance -> GetParam(GUID_TimeSignature, 0xFFFFFFFF, 0, uTimeInMusicFormat, &uNextInMusicFormat, &uTimeSignature);
		uTimeSignature.mtTime += uTimeInMusicFormat;
	
		vResult = m_opDirectMusicPerformance -> TimeToRhythm(uTimeSignature.mtTime, &uTimeSignature, &(upDirectMusicNoteMessage -> wMeasure), &(upDirectMusicNoteMessage -> bBeat), &(upDirectMusicNoteMessage -> bGrid), &(upDirectMusicNoteMessage -> nOffset));
		
		MUSIC_TIME uDurationInMusicFormat;
		m_opDirectMusicPerformance -> ReferenceToMusicTime(p_opEvent -> m_vData1 * 10, &uDurationInMusicFormat);

		upDirectMusicNoteMessage -> bFlags = (p_opEvent -> GetBaseEventType() == EM_MIDI_EVENT_TYPE_NOTE_ON ? DMUS_NOTEF_NOTEON : 0);
		upDirectMusicNoteMessage -> mtDuration = (p_opEvent -> m_opNoteOff != NULL ? SongTimeToReferenceTime(*(p_opEvent -> m_opNoteOff -> m_vpStart)) - SongTimeToReferenceTime(*(p_opEvent -> m_vpStart)): 990000000);

		upDirectMusicNoteMessage -> bTimeRange = 0;
		upDirectMusicNoteMessage -> bDurRange = 0;
		upDirectMusicNoteMessage -> bVelRange = 0;
		upDirectMusicNoteMessage -> bPlayModeFlags = DMUS_PLAYMODE_FIXED; 
		upDirectMusicNoteMessage -> bSubChordLevel = 0;
		upDirectMusicNoteMessage -> cTranspose = 0;

		//Make sure we apply the key offset only to events that are key-specific...
		//...But only to "off-line" events (ie notes that are stored in the event repository and don't come directly from the MIDI consumer)!
		if((p_opEvent -> m_vBaseType == EM_MIDI_EVENT_TYPE_NOTE_ON || 
			p_opEvent -> m_vBaseType == EM_MIDI_EVENT_TYPE_NOTE_OFF || 
			p_opEvent -> m_vBaseType == EM_MIDI_EVENT_TYPE_POLYPHONIC_AFTERTOUCH) && 
			p_opEvent -> GetPosition() -> GetClip() != NULL)
		{
			int32 vReportedKeyOffset = static_cast<EMMediaMIDITrack*>(static_cast<EMMIDIClip*>(p_opEvent -> GetPosition() -> GetClip()) -> GetTrack()) -> GetKeyOffset();
			int32 vKey = static_cast<int32>(p_opEvent -> m_vData1) + vReportedKeyOffset;

			//We're using fixed play mode, so we can put the exact note number here (we don't have to deal with chords, subchords, etc).
			upDirectMusicNoteMessage -> wMusicValue = (vKey >= -127 && vKey <= 127 ? vKey : (vKey < -127 ? -127 : 127));
			upDirectMusicNoteMessage -> bMidiValue = upDirectMusicNoteMessage -> wMusicValue;
		}
		else
		{
			//We're using fixed play mode, so we can put the exact note number here (we don't have to deal with chords, subchords, etc).
			upDirectMusicNoteMessage -> wMusicValue = p_opEvent -> m_vData1; 
			upDirectMusicNoteMessage -> bMidiValue = upDirectMusicNoteMessage -> wMusicValue;
		}

		//Make sure we apply the velocity offset only to events that have some form of varying velocity... 
		//...But only to "off-line" events (ie notes that are stored in the event repository and don't come directly from the MIDI consumer)!
		if((p_opEvent -> m_vBaseType == EM_MIDI_EVENT_TYPE_NOTE_ON) && 
			p_opEvent -> GetPosition() -> GetClip() != NULL)
		{
			int32 vOldVel = p_opEvent -> m_vData2;
			int32 vReportedVelocityOffset = static_cast<EMMediaMIDITrack*>(static_cast<EMMIDIClip*>(p_opEvent -> GetPosition() -> GetClip()) -> GetTrack()) -> GetVelocityOffset();
			int32 vVelocity = static_cast<int32>(p_opEvent -> m_vData2) + vReportedVelocityOffset;
			upDirectMusicNoteMessage -> bVelocity = (vVelocity >= -127 && vVelocity <= 127 ? vVelocity : (vVelocity < -127 ? -127 : 127));
		}
		else
		{
			upDirectMusicNoteMessage -> bVelocity = p_opEvent -> m_vData2;
		}
	}
	else
	{
		DMUS_MIDI_PMSG* upDirectMusicMIDIMessage = reinterpret_cast<DMUS_MIDI_PMSG*>(upNewMessage);
		upDirectMusicMIDIMessage -> bStatus = p_opEvent -> GetEventType();
		upDirectMusicMIDIMessage -> bByte1 = p_opEvent -> m_vData1;
		upDirectMusicMIDIMessage -> bByte2 = p_opEvent -> m_vData2;
	}
	return upNewMessage;
}

HRESULT EMWinDXMIDIProducer::SendNote(DMUS_PMSG* p_upMessage) 
{
    m_opDirectMusicGraph -> StampPMsg((DMUS_PMSG*) p_upMessage);
	p_upMessage -> pGraph = NULL;
	p_upMessage -> pTool = NULL;
	p_upMessage -> dwVoiceID = 0;
	HRESULT vResult = m_opDirectMusicPerformance -> SendPMsg((DMUS_PMSG*) p_upMessage);
	if(FAILED(vResult))
	{
		m_opDirectMusicPerformance -> FreePMsg((DMUS_PMSG*) p_upMessage);
		return vResult;
	}
	return S_OK;
}

bool EMWinDXMIDIProducer::InitCheckE()
{
	try
	{
		if(m_vIsInitialized)
			return true;

		HRESULT vResult = S_OK;
		IDirectMusic* opDirectMusic = NULL;

		if(! EMWinMIDISystem::Instance() -> CreatePerformanceE(&m_opDirectMusicPerformance))
			return false;

		vResult = m_opDirectMusicPerformance -> QueryInterface(IID_IDirectMusicGraph, (void**) &m_opDirectMusicGraph);
	    if(FAILED(vResult)) 
			return false;

		m_opThread = EMThread::CreateEMThread("EMWinDXMIDIProducerThread", EM_THREAD_REALTIME_PRIORITY, 0);
		m_opThread -> AddListener(this);
		m_opThread -> StartSuspended();
		m_opEvent = EM_new EMMediaMIDIEvent();
		m_vIsInitialized = true;
	}
	catch(...)
	{
		EMDebugger("ERROR! Caught an exception while initializing MIDI producer!");
	}
	return true;
}

bool EMWinDXMIDIProducer::AddPortE(EMRealtimeMIDIOutputDescriptor* p_opOutputDescriptor, void* p_upNativeData)
{
	if(! m_vIsInitialized)
		EMDebugger("ERROR! Can't add MIDI port without first initializing MIDI producer");

	GUID* upPortGUID = static_cast<GUID*>(p_upNativeData);
	try
	{
		if(! m_vIsInitialized)
			return false;

		IDirectMusicPort8* opNewPort = NULL;
		if(! EMWinMIDISystem::Instance() -> CreatePortE(&opNewPort, *upPortGUID))
			return false;
		
		m_oPorts[p_opOutputDescriptor] = opNewPort;
		
		HRESULT vResult = m_opDirectMusicPerformance -> AddPort(opNewPort);
		if(FAILED(vResult))
		{
			m_oPorts[p_opOutputDescriptor] = NULL;
			opNewPort -> Release();
			return false;
		}
		
		m_oNoteStatus[p_opOutputDescriptor] = EM_new bool[128];
		for(uint8 vIndex = 0; vIndex < 128; vIndex++)
			m_oNoteStatus[p_opOutputDescriptor][vIndex] = false;

		m_oPChannels[opNewPort] = m_vPChannelCount;
		vResult = m_opDirectMusicPerformance -> AssignPChannel((++m_vPChannelCount), opNewPort, 0, 0);
		vResult = m_opDirectMusicPerformance -> AssignPChannel((++m_vPChannelCount), opNewPort, 0, 1);
		vResult = m_opDirectMusicPerformance -> AssignPChannel((++m_vPChannelCount), opNewPort, 0, 2);
		vResult = m_opDirectMusicPerformance -> AssignPChannel((++m_vPChannelCount), opNewPort, 0, 3);
		vResult = m_opDirectMusicPerformance -> AssignPChannel((++m_vPChannelCount), opNewPort, 0, 4);
		vResult = m_opDirectMusicPerformance -> AssignPChannel((++m_vPChannelCount), opNewPort, 0, 5);
		vResult = m_opDirectMusicPerformance -> AssignPChannel((++m_vPChannelCount), opNewPort, 0, 6);
		vResult = m_opDirectMusicPerformance -> AssignPChannel((++m_vPChannelCount), opNewPort, 0, 7);
		vResult = m_opDirectMusicPerformance -> AssignPChannel((++m_vPChannelCount), opNewPort, 0, 8);
		vResult = m_opDirectMusicPerformance -> AssignPChannel((++m_vPChannelCount), opNewPort, 0, 9);
		vResult = m_opDirectMusicPerformance -> AssignPChannel((++m_vPChannelCount), opNewPort, 0, 10);
		vResult = m_opDirectMusicPerformance -> AssignPChannel((++m_vPChannelCount), opNewPort, 0, 11);
		vResult = m_opDirectMusicPerformance -> AssignPChannel((++m_vPChannelCount), opNewPort, 0, 12);
		vResult = m_opDirectMusicPerformance -> AssignPChannel((++m_vPChannelCount), opNewPort, 0, 13);
		vResult = m_opDirectMusicPerformance -> AssignPChannel((++m_vPChannelCount), opNewPort, 0, 14);
		vResult = m_opDirectMusicPerformance -> AssignPChannel((++m_vPChannelCount), opNewPort, 0, 15);

		if(FAILED(vResult))
		{
			m_oPorts[p_opOutputDescriptor] = NULL;
			delete m_oNoteStatus[p_opOutputDescriptor];
			m_oFailedPortsToRemove.push_back(opNewPort);
			return false;
		}
		if(m_oPorts[p_opOutputDescriptor] == NULL)
			EMDebugger("ERROR!");

		vResult = m_oPorts[p_opOutputDescriptor] -> Activate(false);
		if(FAILED(vResult))
		{
		}
		
		vResult = m_oPorts[p_opOutputDescriptor] -> Activate(true);
		if(FAILED(vResult))
		{
			m_oPorts[p_opOutputDescriptor] = NULL;
			delete m_oNoteStatus[p_opOutputDescriptor];
			m_oFailedPortsToRemove.push_back(opNewPort);
			return false;
		}

		m_oPortList.push_back(opNewPort);
		bool* vpOns = EM_new bool[16 * 128];
		for(int vBool = 0; vBool < 16 * 128; vBool++)
			vpOns[vBool] = false;
		m_oOns[opNewPort] = vpOns;
	}
	catch(...)
	{
		return false;
	}
	return true;
}

bool EMWinDXMIDIProducer::RemovePortE(EMRealtimeMIDIOutputDescriptor* p_opTrack) //??
{
	if(! m_vIsInitialized)
		EMDebugger("ERROR! MIDI producer can't remove port when not initialized");
	return true;
}

void EMWinDXMIDIProducer::SetEventRepository(EMMediaMIDIEventRepository* p_opRepository)
{
	if(p_opRepository == NULL && m_opMIDIEventRepository != NULL)
		m_opMIDIEventRepository = NULL;
	else
		m_opMIDIEventRepository = p_opRepository;
}

void EMWinDXMIDIProducer::OnThreadCreated(EMThread* p_opThread)
{
	HRESULT vResult = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(FAILED(vResult))
		EMDebugger("ERROR! Failed to initialize COM system in EMWinDXMIDIProducer!");
}

void EMWinDXMIDIProducer::OnThreadKilled(EMThread* p_opThread)
{
	CoUninitialize();
}

void EMWinDXMIDIProducer::StartE()
{
}

void EMWinDXMIDIProducer::StartPlaybackE()
{
	if(! m_vIsInitialized)
//		EMDebugger("ERROR! MIDI producer started without being initialized");
		return;
	if(m_opDirectMusicPerformance == NULL)
		return;

	m_opRunningSemaphore -> Acquire();
	try
	{
		m_opMIDIEventRepository = EMMediaEngine::Instance() -> GetMediaProject() -> GetMIDIEventRepository();
		m_opMediaTimer = EMMediaTimer::Instance();
		m_opDirectMusicPerformance -> GetTime(&m_vRefTmStart, NULL);
		m_vSngTmStart = m_opMediaTimer -> MIDIThenTime();
		IReferenceClock* opClock = EMWinMIDISystem::Instance() -> GetReferenceClock();
		REFERENCE_TIME systemtime = (REFERENCE_TIME) 0;
		opClock -> GetTime(&systemtime);

		m_vSystemTimeWhenStarted = (int64) systemtime; //m_opMediaTimer -> GetSystemTimeWhenStarted();
	
		m_opMIDIEventRepository -> ResetReadPosition();
		m_opNextEventToDeliver = NULL;
		
//		m_opNextEventToDeliver = m_opMIDIEventRepository -> GetNextEvent(EMMediaTimer::Instance() -> MIDIThenTime());
//		if(m_opNextEventToDeliver != NULL)
//			m_vNextEventTime = *(m_opNextEventToDeliver -> m_vpStart);

		m_opRunningSemaphore -> Release();
		m_opThread -> Resume();
		m_opRunningSemaphore -> Acquire();
	}
	catch(...)
	{
		EMDebugger("ERROR! Caught an exception while starting playback in MIDI producer!");
	} 
	m_opRunningSemaphore -> Release();
}

void EMWinDXMIDIProducer::StopPlaybackE()
{
	if(! m_vIsInitialized)
		return;

	m_opRunningSemaphore -> Acquire();
	try
	{
		m_opRunningSemaphore -> Release();
		if(m_opThread != NULL)
			m_opThread -> Suspend();
		m_opRunningSemaphore -> Acquire();
		m_vRefTmStart = 0;
		m_vSngTmStart = 0;
		m_vSystemTimeWhenStarted = 0;
		m_opNextEventToDeliver = NULL;
		if(m_opMIDIEventRepository != NULL)
			m_opMIDIEventRepository -> ResetReadPosition();
	}
	catch(...)
	{
		EMDebugger("ERROR! Caught an exception while stopping playback in MIDI producer!");
	}
	m_opRunningSemaphore -> Release();
}

void EMWinDXMIDIProducer::ThreadRun(EMThread* p_opThread)
{
	HRESULT vResult = E_FAIL;
	IReferenceClock* opClock = EMWinMIDISystem::Instance() -> GetReferenceClock();
	REFERENCE_TIME systemtime = (REFERENCE_TIME) 0;

	int64 vMIDIProcessingTime = EMMediaTimer::Instance() -> MIDIThenTime();

	m_opRunningSemaphore -> Acquire();
	if(m_opNextEventToDeliver == NULL)
		m_opNextEventToDeliver = m_opMIDIEventRepository -> GetNextEvent(vMIDIProcessingTime);

	if(m_opNextEventToDeliver == NULL)
	{
		m_opRunningSemaphore -> Release();
		m_opThread -> Suspend(true);
	}
	else
	{
		bool vShouldDeliver = false;
		opClock -> GetTime(&systemtime);
		if(m_opNextEventToDeliver == NULL)
		{
			m_opRunningSemaphore -> Release();
			return;
		}
		int64 vDeltaTime = (*(m_opNextEventToDeliver -> m_vpStart) - m_vSngTmStart + (EMMediaTimer::Instance() -> GetSystemTimeWhenStarted())) - (EMMediaTimer::Instance() -> RealTime()); //EMMediaTimer::Instance() -> RealTime();

		if(vDeltaTime > 10000)
			vDeltaTime = 10000;			//We should only increase the time.. Don't deliver anything now.
		else if(vDeltaTime <= 10000 && vDeltaTime >= 0)
			vShouldDeliver = true;		//We should actually deliver the buffer this run.
		else if(vDeltaTime < 0)
		{
			vShouldDeliver = true;
			vDeltaTime = 0;
		}

		int32 vWaitTime = static_cast<int32>(vDeltaTime / 1000.0);

		m_opRunningSemaphore -> Release();
		vResult = WaitForSingleObject(m_uAbortSignal, vWaitTime);
		if(vResult == WAIT_TIMEOUT)
		{
			m_opRunningSemaphore -> Acquire();
			int64 vNowFrameNow = EMMediaUtility::Instance() -> TimeToFrames((systemtime - m_vSystemTimeWhenStarted) / 10.0 + m_vSngTmStart, EMMediaUtility::Instance() -> GetSystemAudioFormat());
			int64 vNowFrameTitan = EMMediaTimer::Instance() -> NowFrame();
			int64 vNowFrameDiff = vNowFrameNow - vNowFrameTitan;
			if(vNowFrameDiff > 0 && m_opNextEventToDeliver != NULL)
			{
				int32 vOutputID = -1;
				if(m_opNextEventToDeliver -> GetPosition() -> GetDestination() == NULL)
				{
					if(EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks() -> First(EM_TYPE_MIDI) != NULL)
					{
						EMRealtimeMIDIOutputDescriptor* opOutput = static_cast<EMRealtimeMIDIOutputDescriptor*>(EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks() -> First(EM_TYPE_MIDI) -> GetTrackDestination());
						m_opNextEventToDeliver -> GetPosition() -> SetDestination(opOutput);
						vOutputID = (opOutput == NULL ? -1 : opOutput -> GetID());
					}
					else
						vOutputID = -1;
				}
				else 
					vOutputID = m_opNextEventToDeliver -> GetPosition() -> GetDestination() -> GetID();
				
				if(EMMediaTimer::Instance() -> GetState() == EM_PLAYING || EMMediaTimer::Instance() -> GetState() == EM_RECORDING)
					EMMediaTimer::Instance() -> IncreaseNowFrame(vNowFrameDiff, vOutputID);
			}
			if(m_opNextEventToDeliver != NULL)
			{
				int64 t = *(m_opNextEventToDeliver -> m_vpStart) - EMMediaTimer::Instance() -> MIDIThenTime();
				if(vShouldDeliver)
				{
					DeliverMIDIEvent(m_opNextEventToDeliver);
					m_opNextEventToDeliver = NULL;
				}

				if(t > 0)
					EMMediaTimer::Instance() -> IncreaseMIDIThenTime(t);
			}
			m_opRunningSemaphore -> Release();
		}
		else
		{
			m_opThread -> Suspend(true);
		}
	}	
}

void EMWinDXMIDIProducer::Flush()
{
	m_opRunningSemaphore -> Acquire();
	m_opNextEventToDeliver = NULL;
	m_opRunningSemaphore -> Release();
}

bool EMWinDXMIDIProducer::DeliverMIDIEvent(EMMediaMIDIEvent* p_opEvent)
{
	IDirectMusicPort8* opTargetPort = m_oPorts[p_opEvent -> GetPosition() -> GetDestination()];
	DMUS_PMSG* upMessage = ConvertToDirectMusicMessage(p_opEvent, opTargetPort); //(vRefTmTargetTimeOfEvent, vIsNote);

	//Store the "lingering-notes-information", so we can kill (NOTE OFF) the music later, if stopped in the middle of sounding notes...
	if(upMessage != NULL)
	{
		if(p_opEvent -> m_vBaseType == EM_MIDI_EVENT_TYPE_NOTE_ON || p_opEvent -> m_vBaseType == EM_MIDI_EVENT_TYPE_NOTE_OFF)
		{
 			int32 vIndex = (p_opEvent -> GetPosition() -> GetChannel() * 128) + ((DMUS_NOTE_PMSG*) upMessage) -> wMusicValue; //p_opEvent -> m_vData1;
			bool vFlag = (p_opEvent -> m_vBaseType == EM_MIDI_EVENT_TYPE_NOTE_ON ? true : false);
			(m_oOns[opTargetPort])[vIndex] = vFlag;
		}

		if(EMMediaTimer::Instance() -> GetState() == EM_PLAYING || EMMediaTimer::Instance() -> GetState() == EM_RECORDING)
		{
			if(p_opEvent -> GetPosition() != NULL)
				if(p_opEvent -> GetPosition() -> GetClip() != NULL)
				{
					eo << static_cast<EMMediaMIDITrack*>(static_cast<EMMIDIClip*>(p_opEvent -> GetPosition() -> GetClip()) -> GetTrack()) -> GetID() << ef;
					static_cast<EMMediaMIDITrack*>(static_cast<EMMIDIClip*>(p_opEvent -> GetPosition() -> GetClip()) -> GetTrack()) -> MIDIEventReceived(NULL, *p_opEvent -> m_vpStart, p_opEvent -> m_vDataLength, &(p_opEvent -> m_vBaseType));
				}
			SendNote(upMessage);
		}
		else
			m_opDirectMusicPerformance -> FreePMsg((DMUS_PMSG*) upMessage);
	}
	return true;
}

void EMWinDXMIDIProducer::SetActiveMIDIRoutingOutputChannel(uint32 p_vMIDIChannel)
{
	if(m_vIsInitialized)
	{
		m_opRunningSemaphore -> Acquire();
		m_vActiveRoutingOutputChannel = p_vMIDIChannel;
		m_opRunningSemaphore -> Release();
	}
}

uint32 EMWinDXMIDIProducer::GetActiveMIDIRoutingOutputChannel() const
{
	return m_vActiveRoutingOutputChannel;
}

bool EMWinDXMIDIProducer::SendEvent(EMRealtimeMIDIOutputDescriptor* p_opReceiver, uint64 p_vSngTmEventTime, uint64 p_vLengthBytes, void* p_vpData, uint32 p_vMIDIChannel)
{
	return SendEvent(m_oPorts[p_opReceiver], p_vSngTmEventTime, p_vLengthBytes, p_vpData, p_vMIDIChannel);
}

bool EMWinDXMIDIProducer::SendEvent(EMRealtimeMIDIOutputDescriptor* p_opReceiver, uint64 p_vSngTmEventTime, uint64 p_vLengthBytes, void* p_vpData)
{
	return SendEvent(m_oPorts[p_opReceiver], p_vSngTmEventTime, p_vLengthBytes, p_vpData, m_vActiveRoutingOutputChannel);
}

bool EMWinDXMIDIProducer::SendEvent(IDirectMusicPort8* p_opPortOut, uint64 p_vSngTmEventTime, uint64 p_vLengthBytes, void* p_vpData, uint32 p_vMIDIChannel)
{
	if(! m_vIsInitialized)
		EMDebugger("ERROR! MIDI producer can't send any event if it's not initialized first");

	m_opRunningSemaphore -> Acquire();
	if(p_opPortOut == NULL)
	{
		m_opRunningSemaphore -> Release();
		return false;
	}

	m_opEvent -> Set(p_vSngTmEventTime, p_vLengthBytes, p_vpData);
	EMMediaMIDIEventPosition* opPos = m_opEvent -> GetPosition();
	opPos -> SetChannel(p_vMIDIChannel);
	*(m_opEvent -> m_vpStart) = 0;
	DMUS_PMSG* upMessage = ConvertToDirectMusicMessage(m_opEvent, p_opPortOut);
	if(upMessage != NULL)
	{
		if(p_vSngTmEventTime == 0)
			upMessage -> dwFlags = DMUS_PMSGF_TOOL_IMMEDIATE;
		SendNote(upMessage);
	}
	m_opRunningSemaphore -> Release();
	return true;
}

void EMWinDXMIDIProducer::NotesOff()
{
	list<IDirectMusicPort8*>::iterator oPort;
	for(oPort = m_oPortList.begin(); oPort != m_oPortList.end(); oPort++)
	{
		IDirectMusicPort8* opCurrentPort = *oPort;
		bool* opOns = m_oOns[opCurrentPort];
		for(int32 vIndex = 0; vIndex < 16 * 128; vIndex++)
		{
			if(opOns[vIndex])
			{
				int32 vChannel = vIndex / 128;
				int32 vKey = vIndex % 128;

				uint8 vpData[3];
				vpData[0] = EM_MIDI_EVENT_TYPE_NOTE_OFF + vChannel;
				vpData[1] = vKey;
				vpData[2] = 0;

				//SendEvent(opCurrentPort, 0, 3, vpData, vChannel);
				m_opEvent -> Set(0, 3, vpData);
				EMMediaMIDIEventPosition* opPos = m_opEvent -> GetPosition();
				opPos -> SetChannel(vChannel);
				*(m_opEvent -> m_vpStart) = 0;
				DMUS_PMSG* upMessage = ConvertToDirectMusicMessage(m_opEvent, opCurrentPort);
				if(upMessage != NULL)
				{
					upMessage -> dwFlags = DMUS_PMSGF_TOOL_IMMEDIATE;
					SendNote(upMessage);
				}
				opOns[vIndex] = false;
			}
		}
	}
}

void EMWinDXMIDIProducer::BroadcastMIDIMessage(uint64 p_vLengthBytes, void* p_vpData)
{
	EMOutputRepository* opOuts = EMOutputRepository::Instance();
	opOuts -> LockContainer();
	opOuts -> Rewind();
	while(opOuts -> Current() != NULL)
	{
		EMOutputDescriptor* opOutputDesc = opOuts -> Current();
		if((opOutputDesc -> GetType() & EM_TYPE_MIDI) > 0)
		{
			EMRealtimeMIDIOutputDescriptor* opMIDIOutput = static_cast<EMRealtimeMIDIOutputDescriptor*>(opOutputDesc);

			//Differentiate between control change messages and other messages.
			//We can assume that one never broadcasts note ons, note offs, pitch bends and other Channel Messages
			//But the control change message (even tho it being a Channel Mode Message) must be able to be broadcasted
			//since the All Notes Off and All Sound Off messages are Channel Mode Messages.
			if(((uint8*) p_vpData)[0] == EM_MIDI_EVENT_TYPE_CONTROL_CHANGE)
			{
				for(uint8 vChannel = 0; vChannel < 16; vChannel++)
					SendEvent(opMIDIOutput, 0, p_vLengthBytes, p_vpData, vChannel);
			}
			else
			{
				SendEvent(opMIDIOutput, 0, p_vLengthBytes, p_vpData);
			}
		}
		opOuts -> Next();
	}
	opOuts -> UnlockContainer();
}

bool EMWinDXMIDIProducer::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	uint8 vStartEvent = EM_MIDI_EVENT_TYPE_START;
	uint8 vContinueEvent = EM_MIDI_EVENT_TYPE_CONTINUE;
	uint8 vStopEvent = EM_MIDI_EVENT_TYPE_STOP;
	switch(p_vMessage)
	{
		case EM_MESSAGE_TIME_WAS_SEEKED:
		{
			BroadcastMIDIMessage(1, &vStopEvent);
			if(EMMediaTimer::Instance() -> NowTime() == 0 && (EMMediaTimer::Instance() -> GetState() == EM_PLAYING || EMMediaTimer::Instance() -> GetState() == EM_RECORDING))
				BroadcastMIDIMessage(1, &vStartEvent);
			else if(EMMediaTimer::Instance() -> GetState() == EM_PLAYING || EMMediaTimer::Instance() -> GetState() == EM_RECORDING)
				BroadcastMIDIMessage(1, &vContinueEvent);
			break;
		}
		case EM_MESSAGE_BEGIN_PLAYORRECORD:
		{
			if(EMMediaTimer::Instance() -> NowTime() == 0)
				BroadcastMIDIMessage(1, &vStartEvent);
			else
				BroadcastMIDIMessage(1, &vContinueEvent);
			break;
		}
		case EM_MESSAGE_STOP_PLAYORRECORD:
		{
			BroadcastMIDIMessage(1, &vStopEvent);
			break;
		}
		case EM_QUIT_PHASE_ONE:
			return false;
		case EM_QUIT_PHASE_TWO:
			break;
		case EM_QUIT_PHASE_THREE:
		{
			PulseEvent(m_uAbortSignal);				
			if(m_opThread != NULL)					
			{										
				m_opThread -> RemoveListener(this);	
				m_opThread -> Kill();				
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