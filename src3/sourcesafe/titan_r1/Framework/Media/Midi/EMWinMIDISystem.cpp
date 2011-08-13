#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMWinMIDISystem.h"

#include "EMMediaEngine.h"
#include "EMCommandRepository.h"
#include "CommandIDs.h"
#include "EMMediaUtility.h"
#include "EMWinMediaUtility.h"
#include "EMWinRealtimeMIDIInputDescriptor.h"
#include "EMWinRealtimeMIDIOutputDescriptor.h"
#include "EMMediaTimer.h"
#include "EMWinMediaTimer.h"
#include "EMListener.h"
#include "EMListenerRepository.h"
#include "EMWinDXMIDIConsumer.h"
#include "EMWinDXMIDIProducer.h"

#include <dmksctrl.h>

EMWinMIDISystem* EMWinMIDISystem::m_opNativeInstance = NULL;

EMWinMIDISystem* EMWinMIDISystem::Instance()
{
	if(m_opNativeInstance == NULL)
		m_opNativeInstance = EM_new EMWinMIDISystem();
	return m_opNativeInstance;
}

void EMWinMIDISystem::Delete()
{
	delete m_opNativeInstance;
	m_opNativeInstance = NULL;
}

EMWinMIDISystem::EMWinMIDISystem()
:	m_opDirectMusic(NULL),
	m_opDirectMusic8(NULL),
	m_opMasterMIDIClock(NULL)
{
	EMMediaTimer::Instance() -> AddListener(this);
	m_opConsumer = EMWinDXMIDIConsumer::Instance();
	m_opProducer = EMWinDXMIDIProducer::Instance();
}

EMWinMIDISystem::~EMWinMIDISystem()
{
	HRESULT vResult = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(FAILED(vResult))
		EMDebugger("ERROR! Failed to initialize COM system in ~EMWinMIDISystem!");
	
	EMMediaTimer::Instance() -> RemoveListener(this);
	if(m_opDirectMusic8 != NULL)
		int32 v = m_opDirectMusic8 -> Release();
	if(m_opDirectMusic != NULL)
	{
		m_opDirectMusic -> Activate(false);
		int32 v = m_opDirectMusic -> Release();
	}
	if(m_opMasterMIDIClock != NULL)
		int32 v = m_opMasterMIDIClock -> Release();

	CoUninitialize();
}

bool EMWinMIDISystem::InitCheckE()
{
	m_vMIDISystemIsInitialized = true;
	return true; 
}

bool EMWinMIDISystem::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
		case EM_MESSAGE_BEGIN_PLAYORRECORD:
			break;
		case EM_MESSAGE_STOP_PLAYORRECORD:
		{
			EMWinDXMIDIProducer::Instance() -> NotesOff();
			break;
		}
		case EM_MESSAGE_TIME_WAS_SEEKED:
		{
			EMWinDXMIDIProducer::Instance() -> Flush();
			EMWinDXMIDIProducer::Instance() -> NotesOff();	
			break;
		}
		default:
			break;
	};
	return false;
}

list<EMRealtimeInputDescriptor*>* EMWinMIDISystem::GetMIDIInputs()
{
	HRESULT vResult = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(FAILED(vResult))
		EMDebugger("ERROR! Failed to initialize COM system in EMWinMIDISystem::GetMIDIInputs!");

	list<EMRealtimeInputDescriptor*>* opList = NULL;
	opList = EM_new list<EMRealtimeInputDescriptor*>();

	IDirectMusicPerformance8* opDirectMusicPerformance = NULL;
	vResult = CoCreateInstance(CLSID_DirectMusicPerformance, NULL, CLSCTX_INPROC, IID_IDirectMusicPerformance8, (void**) &opDirectMusicPerformance);
	if(SUCCEEDED(vResult))
	{
		vResult = opDirectMusicPerformance -> Init(&m_opDirectMusic, NULL, NULL);
		if(FAILED(vResult))
			EMDebugger("ERROR! Failed to initialize performance in EMWinMIDISystem::GetMIDIInputs()");

		if(m_opDirectMusic8 == NULL)
		{
			vResult = m_opDirectMusic -> QueryInterface(IID_IDirectMusic8, (void**) &m_opDirectMusic8);
			if(FAILED(vResult))
				EMDebugger("ERROR! Failed to get interface to IID_IDirectMusic8 in EMWinMIDISystem::GetMIDIInputs()");
			
			vResult = m_opDirectMusic8 -> SetMasterClock(CLSID_SystemClock);
			vResult = m_opDirectMusic8 -> GetMasterClock(NULL, &m_opMasterMIDIClock);
			IReferenceClock* opTemp = static_cast<IReferenceClock*>(static_cast<EMWinMediaTimer*>(EMMediaTimer::Instance()) -> GetNativeReferenceClock());

//			if(static_cast<EMWinMediaTimer*>(EMMediaTimer::Instance()) -> GetNativeReferenceClock() == NULL)
//				static_cast<EMWinMediaTimer*>(EMMediaTimer::Instance()) -> SetNativeReferenceClock(m_opMasterMIDIClock);
		}

		DMUS_PORTCAPS uPortCapabilities;
		memset(&uPortCapabilities, 0, sizeof(DMUS_PORTCAPS));
		uPortCapabilities.dwSize = sizeof(DMUS_PORTCAPS);
		DWORD vPortNum = 0;
		vResult = m_opDirectMusic8 -> EnumPort(vPortNum, &uPortCapabilities);
		if(FAILED(vResult))
			EMDebugger("ERROR! Failed to enumerator MIDI devices!");

		while(vResult == S_OK)
		{
			WCHAR* vpName = uPortCapabilities.wszDescription;
			string oDeviceName = ((EMWinMediaUtility*) EMMediaUtility::Instance()) -> UnicodeToString(vpName);

			if(uPortCapabilities.dwClass == DMUS_PC_INPUTCLASS && 
				((uPortCapabilities.dwFlags & DMUS_PC_EXTERNAL) == DMUS_PC_EXTERNAL))
			{
/*if(oDeviceName == string("Layla24 MIDI") || 
oDeviceName == string("MIDI Yoke NT:  1 [Emulated]") ||
oDeviceName == string("MIDI Yoke NT:  2 [Emulated]") ||
oDeviceName == string("MIDI Yoke NT:  3 [Emulated]") ||
oDeviceName == string("MIDI Yoke NT:  4 [Emulated]") ||
oDeviceName == string("MIDI Yoke NT:  5 [Emulated]") ||
oDeviceName == string("MIDI Yoke NT:  6 [Emulated]") ||
oDeviceName == string("MIDI Yoke NT:  7 [Emulated]") ||
oDeviceName == string("MIDI Yoke NT:  8 [Emulated]") ||
oDeviceName == string("Layla24 MIDI [Emulated]") ||
oDeviceName == string("SB Live! MIDI UART [Emulated]"))
{*/
				EMRealtimeInputDescriptor* opDescriptor = NULL;
				opDescriptor = EM_new EMWinRealtimeMIDIInputDescriptor(&(uPortCapabilities.guidPort));
		
				try
				{
					if(! opDescriptor -> InitCheckE())
					{
						delete opDescriptor;
					}
					else
					{
						EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_DISPLAY_SPLASH_SCREEN_TEXT, (void*) string(string("Initializing I/O: Incorporating \"") + oDeviceName + string("\"")).c_str(), NULL, NULL);
						opDescriptor -> SetName(oDeviceName);
						opList -> push_back(opDescriptor);
					} 
				}
				catch(...)
				{
					//MessageBox(NULL, string(string("Failed to initialize MIDI IN device \"") + oDeviceName + string("\".")).c_str(), "Error!", MB_OK | MB_ICONERROR);
					delete opDescriptor;
				}
			}
//}
			vPortNum++;
			vResult = m_opDirectMusic8 -> EnumPort(vPortNum, &uPortCapabilities);
		} 
		opDirectMusicPerformance -> Release();
		opDirectMusicPerformance = NULL;
	}
	else
		EMDebugger("ERROR! Failed to get IDirectMusicPerformance interface!");

	CoUninitialize();
	return opList;
}

list<EMRealtimeOutputDescriptor*>* EMWinMIDISystem::GetMIDIOutputs()
{
	HRESULT vResult = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(FAILED(vResult))
		EMDebugger("ERROR! Failed to initialize COM system in EMWinMIDISystem::GetMIDIOutputs!");

	list<EMRealtimeOutputDescriptor*>* opList = NULL;
	opList = EM_new list<EMRealtimeOutputDescriptor*>();

	IDirectMusicPerformance8* opDirectMusicPerformance = NULL;
 
	vResult = CoCreateInstance(CLSID_DirectMusicPerformance, NULL, CLSCTX_INPROC, IID_IDirectMusicPerformance8, (void**) &opDirectMusicPerformance);
	if(SUCCEEDED(vResult))
	{
		vResult = opDirectMusicPerformance -> Init(&m_opDirectMusic, NULL, NULL);
		if(FAILED(vResult))
			EMDebugger("ERROR! Failed to initialize performance in EMWinMIDISystem::GetMIDIInputs()");

		if(m_opDirectMusic8 == NULL)
		{
			vResult = m_opDirectMusic -> QueryInterface(IID_IDirectMusic8, (void**) &m_opDirectMusic8);
			if(FAILED(vResult))
				EMDebugger("ERROR! Failed to get interface to IID_IDirectMusic8 in EMWinMIDISystem::GetMIDIInputs()");

//			vResult = m_opDirectMusic8 -> GetMasterClock(NULL, &m_opMasterMIDIClock);
			vResult = m_opDirectMusic8 -> SetMasterClock(CLSID_SystemClock);
			vResult = m_opDirectMusic8 -> GetMasterClock(NULL, &m_opMasterMIDIClock);
			IReferenceClock* opTemp = static_cast<IReferenceClock*>(static_cast<EMWinMediaTimer*>(EMMediaTimer::Instance()) -> GetNativeReferenceClock());
		}

		DMUS_PORTCAPS uPortCapabilities;
		uPortCapabilities.dwSize = sizeof(DMUS_PORTCAPS);
		DWORD vPortNum = 0;
		vResult = m_opDirectMusic8 -> EnumPort(vPortNum, &uPortCapabilities);
		if(FAILED(vResult))
			EMDebugger("ERROR! Failed to enumerator MIDI devices!");

		while(vResult == S_OK) // && opList -> size() < 1)
		{
			if(uPortCapabilities.dwClass == DMUS_PC_OUTPUTCLASS && ((uPortCapabilities.dwFlags & DMUS_PC_EXTERNAL) == DMUS_PC_EXTERNAL))
			{
				WCHAR* vpName = uPortCapabilities.wszDescription;
				string oDeviceName = ((EMWinMediaUtility*) EMMediaUtility::Instance()) -> UnicodeToString(vpName);

//if(oDeviceName == "sadf")
//{
				EMRealtimeOutputDescriptor* opDescriptor = NULL;
				opDescriptor = EM_new EMWinRealtimeMIDIOutputDescriptor(&(uPortCapabilities.guidPort));
			
				int32 v = uPortCapabilities.dwMaxChannelGroups;
				int32 vv = uPortCapabilities.dwMaxVoices;

				if(! opDescriptor -> InitCheckE())
				{
					//MessageBox(NULL, string(string("Failed to initialize MIDI OUT device \"") + oDeviceName + string("\".")).c_str(), "Error!", MB_OK | MB_ICONERROR);
					delete opDescriptor;
				}
				else
				{
					EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_DISPLAY_SPLASH_SCREEN_TEXT, (void*) string(string("Initializing I/O: Incorporating \"") + oDeviceName + string("\"")).c_str(), NULL, NULL);
					opDescriptor -> SetName(oDeviceName);
					opList -> push_back(opDescriptor);
				} 
//}
			}

			vPortNum++;
			vResult = m_opDirectMusic8 -> EnumPort(vPortNum, &uPortCapabilities);
		}
		opDirectMusicPerformance -> Release();
	}
	else
		EMDebugger("ERROR! Failed to get IDirectMusicPerformance interface!");

	CoUninitialize();

	return opList;
}

/*IDirectMusicPort8* EMWinMIDISystem::SearchForMIDIDevice(uint32 p_vDeviceNum)
{
	DMUS_PORTCAPS sDeviceCapabilities;
	sDeviceCapabilities.dwSize = sizeof(DMUS_PORTCAPS);
	DWORD vPortNum = 0;
	HRESULT vResult = m_opDirectMusic8 -> EnumPort(vPortNum, &sDeviceCapabilities);
	if(FAILED(vResult))
	{
		return NULL;
	}

	while(vResult == S_OK)
	{
		if(vPortNum == p_vDeviceNum)
		{
			IDirectMusicPort8* opMIDIDevice = NULL;
			DMUS_PORTPARAMS8 sDeviceParams;
 

			sDeviceParams.dwSize = sizeof(DMUS_PORTPARAMS8);
			sDeviceParams.dwValidParams = DMUS_PORTPARAMS_SHARE;
			sDeviceParams.fShare = true;

			vResult = m_opDirectMusic8 -> CreatePort(sDeviceCapabilities.guidPort, &sDeviceParams, &opMIDIDevice, NULL);
			if(SUCCEEDED(vResult))
				return opMIDIDevice;
			else
			{
				switch(vResult)
				{
				case DMUS_E_DSOUND_NOT_SET:
					break;
				case E_INVALIDARG:
					break;
				case CLASS_E_NOAGGREGATION:
					break;
				case E_NOINTERFACE:
					break;
				case E_OUTOFMEMORY:
					break;
				case E_POINTER:
					break;
				default:

					char vpTxt[MAX_ERROR_TEXT_LEN];
					uint16 vNum = AMGetErrorText(vResult, vpTxt, MAX_ERROR_TEXT_LEN);
					string oError = string(vpTxt); // ((EMWinMediaUtility*) EMMediaUtility::Instance()) -> UnicodeToString(v);
					//MessageBox(NULL, oError.c_str(), "Error", MB_OK);
					break;
				};
			}
		}
		vPortNum++;
		vResult = m_opDirectMusic8 -> EnumPort(vPortNum, &sDeviceCapabilities);
	}
	return NULL;
} */

bool EMWinMIDISystem::CreatePerformanceE(IDirectMusicPerformance8** p_oppPerformance)
{
//	_ASSERT(p_oppPerformance != NULL);
	HRESULT vResult = S_OK;
	vResult = CoCreateInstance(CLSID_DirectMusicPerformance, NULL, CLSCTX_INPROC, IID_IDirectMusicPerformance8, (void**) p_oppPerformance);
	vResult = (*p_oppPerformance) -> Init(&m_opDirectMusic, NULL, NULL);
	if(FAILED(vResult))
		EMDebugger("ERROR! Failed to initialize performance in EMWinMIDISystem::CreatePerformanceAndAddMIDIDevice()");
	if(m_opDirectMusic8 == NULL)
	{
		vResult = m_opDirectMusic -> QueryInterface(IID_IDirectMusic8, (void**) &m_opDirectMusic8);
		if(FAILED(vResult))
			EMDebugger("ERROR! Failed to get interface to IID_IDirectMusic8 in EMWinMIDISystem::GetMIDIInputs()");
		
//		vResult = m_opDirectMusic8 -> GetMasterClock(NULL, &m_opMasterMIDIClock);
		vResult = m_opDirectMusic8 -> SetMasterClock(CLSID_SystemClock);
		vResult = m_opDirectMusic8 -> GetMasterClock(NULL, &m_opMasterMIDIClock);
		IReferenceClock* opTemp = static_cast<IReferenceClock*>(static_cast<EMWinMediaTimer*>(EMMediaTimer::Instance()) -> GetNativeReferenceClock());
	}
//	_ASSERT((*p_oppPerformance) != NULL);
	return true;
}

//bool EMWinMIDISystem::CreatePortE(IDirectMusicPort8** p_oppPort, uint32 p_vDeviceNumber)
bool EMWinMIDISystem::CreatePortE(IDirectMusicPort8** p_oppPort, GUID p_uPortGUID)
{
	DMUS_PORTPARAMS8 sDeviceParams;
	memset(&sDeviceParams, 0, sizeof(DMUS_PORTPARAMS8));

	sDeviceParams.dwSize = sizeof(DMUS_PORTPARAMS8);
	sDeviceParams.dwValidParams = DMUS_PORTPARAMS_SHARE | DMUS_PORTPARAMS_CHANNELGROUPS | DMUS_PORTPARAMS_EFFECTS;
	sDeviceParams.fShare = false;
	sDeviceParams.dwChannelGroups = 1;
	sDeviceParams.dwEffectFlags = DMUS_EFFECT_NONE;

	HRESULT vResult = m_opDirectMusic8 -> CreatePort(p_uPortGUID, &sDeviceParams, p_oppPort, NULL);
	if(vResult != S_OK)
	{
		switch(vResult)
		{
		case S_FALSE:
			eo << "" << ef;
			vResult = (*p_oppPort) -> SetNumChannelGroups(1);
			return true;
			break;
		case DMUS_E_DSOUND_NOT_SET:
			eo << "" << ef;
			break;
		case E_INVALIDARG:
			eo << "" << ef;
			break;
		case CLASS_E_NOAGGREGATION:
			eo << "" << ef;
			break;
		case E_NOINTERFACE:
			eo << "" << ef;
			break;
		case E_OUTOFMEMORY:
			eo << "" << ef;
			break;
		case E_POINTER:
			eo << "" << ef;
			break;
		default:
			eo << "" << ef;
			break;
		};
		return false;
	}
	vResult = (*p_oppPort) -> SetNumChannelGroups(1);
//	_ASSERT((*p_oppPort) != NULL);
	return true;
}

IDirectMusic8* EMWinMIDISystem::GetDirectMusicObject() const
{
//	_ASSERT(m_opDirectMusic8 != NULL);
	return m_opDirectMusic8;
}

IReferenceClock* EMWinMIDISystem::GetReferenceClock() const
{
	return m_opMasterMIDIClock;
}

void EMWinMIDISystem::StartMIDISystemE()
{
	m_opConsumer -> StartE();
	m_opProducer -> StartE();
}

#endif