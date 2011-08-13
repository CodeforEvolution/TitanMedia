#include "EMGlobals.h"

#include "EMMediaMetronome.h"

#include "EMApplication.h" //For being able to listen for quit-messages!
#include "EMMediaDebugLog.h"
#include "EMMediaEngine.h"
#include "EMMediaGlobals.h"
#include "EMMediaTimer.h"
#include "EMMessages.h"
#include "EMMIDIGlobals.h"
#include "EMOutputRepository.h"
#include "EMRealtimeMIDIOutputDescriptor.h"
#include "EMSemaphore.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMThread.h"

#ifdef PLATFORM_WINDOWS
#include "EMWinMediaMetronome.h"
#endif

#include <math.h>

EMMediaMetronome* EMMediaMetronome::m_opInstance = NULL;

EMMediaMetronome* EMMediaMetronome::Instance()
{
	if(m_opInstance == NULL)
#ifdef PLATFORM_WINDOWS
		m_opInstance = EM_new EMWinMediaMetronome();
#endif
	return m_opInstance;
}

void EMMediaMetronome::Delete()
{
	delete m_opInstance;
	m_opInstance = NULL;
}

EMMediaMetronome::EMMediaMetronome()
	:	m_vClickNumber(0),
		m_vSystemTimeWhenStarted(0),
		m_vSongTimeWhenStarted(0),
		m_vBPM(120),
		m_vSignatureMajor(4),
		m_vSignatureMinor(4),
		m_uSleepSemaphore(NULL),
		m_vIsEnabled(false),
		m_opProtectionSemaphore(NULL),

		m_vUsePCSpeaker(true),
		m_vUseMIDINote(false),
		m_vDuringPlayback(false),
		m_vDuringRecording(false),
		m_oDevice(""),
		m_vMIDIChannel(0),
		m_vHighKey(0),
		m_vHighVelocity(0),
		m_vHighDuration(0),
		m_vLowKey(0),
		m_vLowVelocity(0),
		m_vLowDuration(0),
		m_vCountIn(0),
		m_opMetronomeMIDIDevice(NULL),

		m_vLastTickTime(-1)
{
	EMApplication::Instance() -> AddListener(this); //Listen for quit-messages!
	m_opThread = EMThread::CreateEMThread("Metronome", EM_THREAD_HIGH_REALTIME_PRIORITY, 0);
	m_opThread -> AddListener(this);
	m_opThread -> StartSuspended(); 
	EMMediaTimer::Instance() -> AddListener(this);
	EMMediaEngine::Instance() -> GetSettingsRepository() -> AddListener(this);
	m_vIsEnabled = *static_cast<bool*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_METRONOME_ENABLED));
	m_vBPM = *static_cast<float*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_TEMPO));
	string oSignature = *static_cast<string*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_SIGNATURE));
	sscanf(oSignature.c_str(), "%d/%d", &m_vSignatureMajor, &m_vSignatureMinor);
	m_vUsePCSpeaker = *static_cast<bool*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_METRONOME_USE_PC_SPEAKER));
	m_vUseMIDINote = *static_cast<bool*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_METRONOME_USE_MIDI_NOTE));
	m_vDuringPlayback = *static_cast<bool*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_METRONOME_DURING_PLAYBACK));
	m_vDuringRecording = *static_cast<bool*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_METRONOME_DURING_RECORDING));
	m_oDevice = string(static_cast<string*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_METRONOME_MIDI_DEVICE)) -> c_str());
	m_vMIDIChannel = static_cast<uint32>(*static_cast<int32*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_METRONOME_MIDI_CHANNEL)));
	m_vHighKey = static_cast<uint8>(*static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_METRONOME_MIDI_NOTE_HIGH_KEY)));
	m_vHighVelocity = static_cast<uint8>(*static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_METRONOME_MIDI_NOTE_HIGH_VELOCITY)));
	m_vHighDuration = *static_cast<int64*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_METRONOME_MIDI_NOTE_HIGH_DURATION));
	m_vLowKey = static_cast<uint8>(*static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_METRONOME_MIDI_NOTE_LOW_KEY)));
	m_vLowVelocity = static_cast<uint8>(*static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_METRONOME_MIDI_NOTE_LOW_VELOCITY)));
	m_vLowDuration = *static_cast<int64*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_METRONOME_MIDI_NOTE_LOW_DURATION));
	m_vCountIn = *static_cast<int32*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_METRONOME_COUNT_IN));
	EMRealtimeMIDIOutputDescriptor* opMIDIOut = static_cast<EMRealtimeMIDIOutputDescriptor*>(EMOutputRepository::Instance() -> SearchForName(m_oDevice));
	m_opMetronomeMIDIDevice = (opMIDIOut != NULL && opMIDIOut -> GetType() == EM_TYPE_MIDI ? opMIDIOut : NULL);
	m_uSleepSemaphore = CreateSemaphore(NULL, 0, 1, "Metronome sleep semaphore");
	m_opProtectionSemaphore = EMSemaphore::CreateEMSemaphore("Metronome protection sem", 1, 1);
}

EMMediaMetronome::~EMMediaMetronome()
{
	EMApplication::Instance() -> RemoveListener(this); //Stop listening for quit-messages!
	EMMediaTimer::Instance() -> RemoveListener(this);
	EMMediaEngine::Instance() -> GetSettingsRepository() -> RemoveListener(this);
//	m_opThread -> Kill();					//Moved to MessageReceived EM_QUIT_PHASE_THREE
//	m_opThread -> RemoveListener(this);		//Moved to MessageReceived EM_QUIT_PHASE_THREE
//	delete m_opThread;						//Moved to MessageReceived EM_QUIT_PHASE_THREE
	CloseHandle(m_uSleepSemaphore);
	delete m_opProtectionSemaphore;
}

bool EMMediaMetronome::MessageReceived(EMListenerRepository* p_opWho, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
		case EM_MESSAGE_BEGIN_PLAYORRECORD:
		{
//			m_opProtectionSemaphore -> Acquire();
			m_vLastTickTime = -1;
//			m_opProtectionSemaphore -> Release();
			return true;
		}
		case EM_MESSAGE_BEGIN_BUFFER_UP:
		{
//			m_opProtectionSemaphore -> Acquire();
			m_vLastTickTime = -1;
//			m_opProtectionSemaphore -> Release();
			this -> Start();
			return true;
		}
		case EM_MESSAGE_TIME_WAS_SEEKED:
		{
			ReleaseSemaphore(m_uSleepSemaphore, 1, NULL);
			m_opProtectionSemaphore -> Acquire();
			m_vLastTickTime = -1;
			m_vSystemTimeWhenStarted = EMMediaTimer::Instance() -> GetSystemTimeWhenStarted();
			m_vSongTimeWhenStarted = EMMediaTimer::Instance() -> MetronomeNowTime();
			m_vClickNumber = 0;
			m_opProtectionSemaphore -> Release();
			return true;
		}
		case EM_MESSAGE_STOP_PLAYORRECORD:
		{
			Stop();
			//BEGIN TEST - COPIED FROM EM_MESSAGE_TIME_WAS_SEEKED
			m_opProtectionSemaphore -> Acquire();
			m_vLastTickTime = -1;
			m_vSystemTimeWhenStarted = EMMediaTimer::Instance() -> GetSystemTimeWhenStarted();
			m_vSongTimeWhenStarted = EMMediaTimer::Instance() -> MetronomeNowTime();
			m_vClickNumber = 0;
			m_opProtectionSemaphore -> Release();
			//END TEST - COPIED FROM EM_MESSAGE_TIME_WAS_SEEKED
			return true;
		}
		case EM_SETTING_UPDATED:
		{
			float vBPM = *static_cast<float*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_TEMPO));
			bool vEnabled = *static_cast<bool*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_METRONOME_ENABLED));
			string oSignature = *static_cast<string*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_SIGNATURE));
			bool vUsePCSpeaker = *static_cast<bool*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_METRONOME_USE_PC_SPEAKER));
			bool vUseMIDINote = *static_cast<bool*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_METRONOME_USE_MIDI_NOTE));
			bool vDuringPlayback = *static_cast<bool*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_METRONOME_DURING_PLAYBACK));
			bool vDuringRecording = *static_cast<bool*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_METRONOME_DURING_RECORDING));
			string oDevice = string(static_cast<string*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_METRONOME_MIDI_DEVICE)) -> c_str());
			uint32 vMIDIChannel = static_cast<uint32>(*static_cast<int32*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_METRONOME_MIDI_CHANNEL)));
			uint8 vHighKey = static_cast<uint8>(*static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_METRONOME_MIDI_NOTE_HIGH_KEY)));
			uint8 vHighVelocity = static_cast<uint8>(*static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_METRONOME_MIDI_NOTE_HIGH_VELOCITY)));
			int64 vHighDuration = *static_cast<int64*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_METRONOME_MIDI_NOTE_HIGH_DURATION));
			uint8 vLowKey = static_cast<uint8>(*static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_METRONOME_MIDI_NOTE_LOW_KEY)));
			uint8 vLowVelocity = static_cast<uint8>(*static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_METRONOME_MIDI_NOTE_LOW_VELOCITY)));
			int64 vLowDuration = *static_cast<int64*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_METRONOME_MIDI_NOTE_LOW_DURATION));
			int32 vCountIn = *static_cast<int32*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_METRONOME_COUNT_IN));
			
			int32 vSignatureMajor = 1;
			int32 vSignatureMinor = 1;
			sscanf(oSignature.c_str(), "%d/%d", &vSignatureMajor, &vSignatureMinor);
			if(vBPM != m_vBPM || 
				vEnabled != m_vIsEnabled || 
				vSignatureMajor != m_vSignatureMajor || 
				vSignatureMinor != m_vSignatureMinor ||
				m_vUsePCSpeaker != vUsePCSpeaker ||
				m_vUseMIDINote != vUseMIDINote ||
				m_vDuringPlayback != vDuringPlayback ||
				m_vDuringRecording != vDuringRecording ||
				m_oDevice != oDevice ||
				m_vMIDIChannel != vMIDIChannel ||
				m_vHighKey != vHighKey ||
				m_vHighVelocity != vHighVelocity ||
				m_vHighDuration != vHighDuration ||
				m_vLowKey != vLowKey ||
				m_vLowVelocity != vLowVelocity ||
				m_vLowDuration != vLowDuration ||
				m_vCountIn != vCountIn)
			{
				m_opProtectionSemaphore -> Acquire();
				m_vLastTickTime = -1;
				m_vBPM = vBPM;
				m_vIsEnabled = vEnabled;
				m_vSignatureMajor = vSignatureMajor;
				m_vSignatureMinor = vSignatureMinor;
				m_vUsePCSpeaker = vUsePCSpeaker;
				m_vUseMIDINote = vUseMIDINote;
				m_vDuringPlayback = vDuringPlayback;
				m_vDuringRecording = vDuringRecording;
				m_oDevice = oDevice.c_str();
				m_vMIDIChannel = vMIDIChannel;
				m_vHighKey = vHighKey;
				m_vHighVelocity = vHighVelocity;
				m_vHighDuration = vHighDuration;
				m_vLowKey = vLowKey;
				m_vLowVelocity = vLowVelocity;
				m_vLowDuration = vLowDuration;
				m_vCountIn = vCountIn;
				m_opProtectionSemaphore -> Release();

				EMRealtimeMIDIOutputDescriptor* opMIDIOut = static_cast<EMRealtimeMIDIOutputDescriptor*>(EMOutputRepository::Instance() -> SearchForName(m_oDevice));
				if(opMIDIOut == NULL)
					opMIDIOut = static_cast<EMRealtimeMIDIOutputDescriptor*>(EMOutputRepository::Instance() -> First(EM_TYPE_MIDI));

				if(opMIDIOut != NULL)
				{
					m_opProtectionSemaphore -> Acquire();
					m_opMetronomeMIDIDevice = opMIDIOut;
					m_opProtectionSemaphore -> Release();
				}
			}
			return true;
		}
		case EM_QUIT_PHASE_ONE:
			return false;
		case EM_QUIT_PHASE_TWO:
			break;
		case EM_QUIT_PHASE_THREE:
		{
			Stop();
			m_opThread -> RemoveListener(this);
			m_opThread -> Kill();
			delete m_opThread;
			m_opThread = NULL;
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

void EMMediaMetronome::Start()
{
	m_opProtectionSemaphore -> Acquire();
	m_vSystemTimeWhenStarted = EMMediaTimer::Instance() -> GetSystemTimeWhenStarted();
	m_vSongTimeWhenStarted = EMMediaTimer::Instance() -> MetronomeNowTime();
	m_vClickNumber = 0;
	m_opProtectionSemaphore -> Release();
	m_opThread -> Resume();
}

void EMMediaMetronome::Stop()
{
	ReleaseSemaphore(m_uSleepSemaphore, 1, NULL);
	m_opThread -> Suspend();
	m_opProtectionSemaphore -> Acquire();
	m_vSystemTimeWhenStarted = 0;
	m_vSongTimeWhenStarted = 0;
	m_vClickNumber = 0;
	m_opProtectionSemaphore -> Release();
}

void EMMediaMetronome::ThreadRun(EMThread* p_opThread)
{
	m_opProtectionSemaphore -> Acquire(); //BEGIN CRITICAL SECTION
	int64 vMicroSecondsPerClick = ceil(60000000.0 / (m_vBPM * m_vSignatureMinor / 4));
	int64 vMetronomeTimeSngTm = (EMMediaTimer::Instance() -> GetState() == EM_PLAYING || EMMediaTimer::Instance() -> GetState() == EM_RECORDING ? EMMediaTimer::Instance() -> NowTime() : EMMediaTimer::Instance() -> MetronomeNowTime());

	int64 vDifference = vMetronomeTimeSngTm - m_vSongTimeWhenStarted; //now - whattimewaswhenwestarted

	int64 vThisClickNum = (int64) ceil(((float) vMetronomeTimeSngTm) / ((float) vMicroSecondsPerClick));
	m_vClickNumber = vThisClickNum % m_vSignatureMajor;
	int64 vTargetTime = vThisClickNum * vMicroSecondsPerClick;
	m_opProtectionSemaphore -> Release(); //END CRITICAL SECTION
	
//	if(vTargetTime > EMMediaTimer::Instance() -> NowTime())
//		vTargetTime = EMMediaTimer::Instance() -> NowTime();

	int64 vSleepTime = vTargetTime - vMetronomeTimeSngTm;

//	if(vSleepTime <= 0)
//		eo << "?" << ef;

	while(vSleepTime > 0)
	{
		HRESULT vResult = WaitForSingleObject(m_uSleepSemaphore, (vSleepTime) / 1000);
		if(vResult == WAIT_OBJECT_0)
			return; //Skip the tone production this run, since we've been told to stop! 
		
		//Make sure that the ticks don't come until we've passed the time we waited for.
		//I.e. handle the fact that the song time sometimes slows down due to heavy load.
		int64 vTimeNow = (EMMediaTimer::Instance() -> GetState() == EM_PLAYING || EMMediaTimer::Instance() -> GetState() == EM_RECORDING ? EMMediaTimer::Instance() -> NowTime() : EMMediaTimer::Instance() -> MetronomeNowTime());
		if(vTimeNow > vTargetTime || EMMediaTimer::Instance() ->GetState() == EM_COUNTING_IN_FOR_PLAYBACK || EMMediaTimer::Instance() ->GetState() == EM_COUNTING_IN_FOR_RECORDING)
			break;
		else 
			vSleepTime = vTargetTime - vTimeNow;
		if(vSleepTime <= 0)
			vSleepTime = 1000;
	}

	//It is now safe to assume that it's time to produce a tick!
	
	if(vTargetTime != m_vLastTickTime && EMMediaTimer::Instance() -> GetState() != EM_STOPPED && ((EMMediaTimer::Instance() -> GetState() == EM_PLAYING && m_vDuringPlayback) || (EMMediaTimer::Instance() -> GetState() == EM_RECORDING && m_vDuringRecording)))
	{
	//	eo << "Metronome slept for " << vSleepTime / 1000.0 << " milliseconds..." << ef;
		m_opProtectionSemaphore -> Acquire();
		if(m_vIsEnabled)
		{
			int32 vFrequency = 0;
			if((m_vClickNumber % m_vSignatureMajor) == 0)
			{
				//eo << "HIGH CLICK. Start = " << m_vSongTimeWhenStarted / 1000.0 << ", MetTmNow = " << vMetronomeTimeSngTm / 1000.0 << ", Sleep tm = " << vSleepTime << ef;
				if(m_vUseMIDINote && m_opMetronomeMIDIDevice != NULL)
					ProduceMIDIBeep(m_opMetronomeMIDIDevice, m_vHighKey, m_vHighVelocity, m_vMIDIChannel, m_vHighDuration);
				if(m_vUsePCSpeaker)
					ProduceSystemBeep(2900);
				//EMMediaTimer::Instance() -> IncreaseMetronomeNowTime(((vThisClickNum + 1) * vMicroSecondsPerClick) - vMetronomeTimeSngTm);
			}
			else 
			{
				//eo << "LOW CLICK. Start = " << m_vSongTimeWhenStarted / 1000.0 << ", MetTmNow = " << vMetronomeTimeSngTm / 1000.0 << ", Sleep tm = " << vSleepTime << ef;
				if(m_vUseMIDINote && m_opMetronomeMIDIDevice != NULL)
					ProduceMIDIBeep(m_opMetronomeMIDIDevice, m_vLowKey, m_vLowVelocity, m_vMIDIChannel, m_vLowDuration);
				if(m_vUsePCSpeaker)
					ProduceSystemBeep(1013);
				//EMMediaTimer::Instance() -> IncreaseMetronomeNowTime(((vThisClickNum + 1) * vMicroSecondsPerClick) - vMetronomeTimeSngTm);
			}
		}
		m_vLastTickTime = vTargetTime;
		m_opProtectionSemaphore -> Release();
	}
	else
		::Sleep(5);
//	else
//		eo << "?" << ef;
	
//	if(vMetronomeTimeSngTm + vSleepTime >= vNormalSongTime)
//	{
//		EMMediaTimer::Instance() -> Start();
//	}
}
