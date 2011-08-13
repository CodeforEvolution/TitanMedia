#include "EMGlobals.h"

#include "EMMediaTimer.h"

#include "CommandIDs.h"
#include "EMApplication.h" //For being able to listen for quit-messages!
#include "EMCommandRepository.h"
#include "EMMediaEngine.h"
#include "EMMediaEngine.h"
#include "EMMediaGlobals.h"
#include "EMMediaProject.h"
#include "EMMediaUtility.h"
#include "EMMessages.h"
#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"
#include "EMRealtimeOutputDescriptor.h"
#include "EMSemaphore.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "SettingIDs.h"

#ifdef PLATFORM_BEOS
#include "EMBeMediaTimer.h"
#endif

#ifdef PLATFORM_WINDOWS
#include "EMWinMediaTimer.h"
#endif

extern "C" __declspec(dllexport) EMMediaTimer* InstanceMediaTimer() { return EMMediaEngine::Instance() -> GetMediaTimer(); };
extern "C" __declspec(dllexport) void DeleteMediaTimer() { EMMediaTimer::Delete(); };

EMMediaTimer* EMMediaTimer::m_opInstance = NULL;

EMMediaTimer* EMMediaTimer::Instance()
{
	if(m_opInstance == NULL)
#ifdef PLATFORM_BEOS
		m_opInstance = EM_new EMBeMediaTimer();
#endif
#ifdef PLATFORM_WINDOWS
		m_opInstance = EM_new EMWinMediaTimer();
#endif

	EMMediaTimer* opTimer = m_opInstance;
	return opTimer;
}

void EMMediaTimer::Delete() //Deleted by MediaEngine
{
	delete m_opInstance;
	m_opInstance = NULL;
}

EMMediaTimer::EMMediaTimer()
	:	m_vID(EMMediaIDManager::MakeID()),
		m_vNow(0),
		m_vAudioThen(0),
		m_vVideoThen(0),
		m_vMIDIThen(0),
		m_vMetronomeThen(0),
		m_eState(EM_STOPPED),
		m_vLoopStart(0),
		m_vLoopEnd(0),
		m_vIsLooped(false),
		m_vSystemTimeWhenStarted(0),
		m_vSystemTimeWhenCountInStarted(0),
		m_vClockMasterID(-1),
		m_opTimeProtectionSemaphore(NULL)
{
	EMApplication::Instance() -> AddListener(this); //Listen for quit-messages!
	m_opTimeProtectionSemaphore = EMSemaphore::CreateEMSemaphore("EMMediaTimer semaphore");
	EMMediaEngine::Instance() -> GetSettingsRepository() -> AddListener(this);

}

EMMediaTimer::~EMMediaTimer()
{
	EMApplication::Instance() -> RemoveListener(this); //Stop listening for quit-messages!
	delete m_opTimeProtectionSemaphore;	
	EMMediaEngine::Instance() -> GetSettingsRepository() -> RemoveListener(this);
}

int32 EMMediaTimer::GetID() const
{
	return m_vID;
}

bool EMMediaTimer::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
		case EM_SETTING_UPDATED:
		{
			m_vLoopStart = EMMediaUtility::Instance() -> TimeToFrames(*static_cast<int64*>(EMMediaEngine::Instance() -> 
				GetSettingsRepository() -> GetSetting(SETTING_REGION_BEGIN)), EMMediaUtility::Instance() -> GetSystemAudioFormat());
			m_vLoopEnd = EMMediaUtility::Instance() -> TimeToFrames(*static_cast<int64*>(EMMediaEngine::Instance() -> 
				GetSettingsRepository() -> GetSetting(SETTING_REGION_END)), EMMediaUtility::Instance() -> GetSystemAudioFormat());
			m_vIsLooped = *static_cast<bool*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_LOOP));
			if(m_vIsLooped && ! (m_vLoopStart <= m_vNow && m_vLoopEnd > m_vNow))
				SeekToFrame(m_vLoopStart);
			//TODO: Fix the bug that makes the playback go mad if the region is changed while the then-time has wrapped (started from the beginning
			//due to the loop) but the now-time hasn't!
			return true;
		}
		case EM_QUIT_PHASE_ONE:
			return false;
		case EM_QUIT_PHASE_TWO:
			break;
		case EM_QUIT_PHASE_THREE:
			break;
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

int64 EMMediaTimer::GetLoopStart() const
{
	return m_vLoopStart;
}

int64 EMMediaTimer::GetLoopEnd() const
{
	return m_vLoopEnd;
}

bool EMMediaTimer::IsLooped() const
{
	return m_vIsLooped;
}

EMMediaTimerState EMMediaTimer::GetState() const
{
	return m_eState;
}

bool EMMediaTimer::SetState(EMMediaTimerState p_eNewState)
{
	if(m_eState == p_eNewState)
		return false;

	switch(p_eNewState)
	{
		case EM_STOPPED:
		{
			Lock();
			m_vAudioThen = m_vNow;
			m_vVideoThen = m_vNow;
			m_vMIDIThen = EMMediaUtility::Instance() -> FramesToTime(m_vNow, EMMediaUtility::Instance() -> GetSystemAudioFormat());
			m_vMetronomeThen = m_vNow;
			m_vSystemTimeWhenStarted = 0;
			m_vSystemTimeWhenCountInStarted = 0;
			m_eState = p_eNewState;
			EMMediaTimerState eOldState = m_eState;
			Unlock();
			Notify(EM_MESSAGE_STOP_PLAYORRECORD);
			if(eOldState == EM_PLAYING) OnStopPlayback();
			else if(eOldState == EM_RECORDING) OnStopRecording();

			//EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_WRITE_STATUS_BAR_MESSAGE, (void*) string("Stopped").c_str());

			break;
		}
		case EM_RECORDING:	//Use same case as EM_PLAYING
		case EM_PLAYING:
		{
			//EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_WRITE_STATUS_BAR_MESSAGE, (void*) string("Started").c_str());

			if((EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingAudio() || EMMediaEngine::Instance() -> 
				GetMediaProject() -> IsRenderingVideo()))// && m_vLoopStart != m_vLoopEnd)
			{
				int64 vLoopStart = m_vLoopStart;
				if(vLoopStart < 0)
					vLoopStart = 0;
				m_vNow = vLoopStart;
				m_vAudioThen = vLoopStart;
				m_vVideoThen = vLoopStart;
				m_vMIDIThen = EMMediaUtility::Instance() -> FramesToTime(vLoopStart, EMMediaUtility::Instance() -> GetSystemAudioFormat());
			}

			Notify(EM_MESSAGE_BEGIN_PLAYORRECORD);
			Lock();
			//EMMediaEngine::Instance() -> GetSettingsRepository() -> AddListener(this);
			m_vSystemTimeWhenStarted = RealTime();
			//m_vSystemTimeWhenCountInStarted = 0;
			m_vMIDIThen = EMMediaUtility::Instance() -> FramesToTime(m_vNow, EMMediaUtility::Instance() -> GetSystemAudioFormat());
			m_eState = p_eNewState;
			Unlock();
			OnStartPlayback();
			break;
		}
		case EM_COUNTING_IN_FOR_RECORDING: //Use same case as EM_COUNTING_IN_FOR_PLAYBACK
		case EM_COUNTING_IN_FOR_PLAYBACK:
		{
			Lock();
			int64 vNowTime = EMMediaUtility::Instance() -> FramesToTime(m_vNow, EMMediaUtility::Instance() -> GetSystemAudioFormat());
			float vBPM = *static_cast<float*>(EMMediaEngine::Instance() -> GetSettingsRepository()-> GetSetting(SETTING_TEMPO));
			int32 vCountInMeasures = 0; //*static_cast<int32*>(EMMediaEngine::Instance() -> GetSettingsRepository()-> GetSetting(SETTING_MIDI_METRONOME_COUNT_IN));
			string oSignature = *static_cast<string*>(EMMediaEngine::Instance() -> GetSettingsRepository()-> GetSetting(SETTING_SIGNATURE));
			int32 vSignatureMajor = 0;
			int32 vSignatureMinor = 0;
			sscanf(oSignature.c_str(), "%d/%d", &vSignatureMajor, &vSignatureMinor);

			float vTimePerQuarter = 60000000.0 / vBPM;
			int32 vQuartersPerMeasure = vSignatureMajor;
			int32 vQuartersPerCountIn = vQuartersPerMeasure * vCountInMeasures;
			int32 vCountInTimeUnAltered = vTimePerQuarter * vQuartersPerCountIn;

			m_vMetronomeThen = vNowTime - (vCountInTimeUnAltered + 0);
			m_vSystemTimeWhenStarted = 0;
			m_vSystemTimeWhenCountInStarted = RealTime();
			m_eState = p_eNewState;

			m_vAudioThen = m_vNow;
			m_vVideoThen = m_vNow;

			Unlock();

			char vpBuff[10];
			int32 vMilliseconds = *static_cast<int32*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_PROCESSING_AHEAD));
			sprintf(vpBuff, "%d", vMilliseconds);
			//EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_WRITE_STATUS_BAR_MESSAGE, (void*) string(string("Now buffering up") + string(vpBuff) + string(" ms...")).c_str());

			Notify(EM_MESSAGE_BEGIN_BUFFER_UP);
			break;
		}
		default:
			m_eState = p_eNewState;
			break;
	};
	return true;
}

bool EMMediaTimer::ClearData()
{
	m_vNow = 0;
	m_vAudioThen = 0;
	m_vVideoThen = 0;
	m_vMIDIThen = 0;
	m_vLoopStart = 0;
	m_vLoopEnd = 0;
	m_vIsLooped = false;
	return true;
}

bool EMMediaTimer::SaveData(EMProjectDataSaver* p_opSaver)
{
	p_opSaver -> SaveUInt64(static_cast<uint64>(m_vNow));
	p_opSaver -> SaveUInt64(static_cast<uint64>(m_vAudioThen));
	p_opSaver -> SaveUInt64(static_cast<uint64>(m_vVideoThen));
	return false;
}

bool EMMediaTimer::LoadData(EMProjectDataLoader* p_opLoader)
{
	m_vNow = static_cast<int64>(p_opLoader -> LoadUInt64());
	m_vAudioThen = static_cast<int64>(p_opLoader -> LoadUInt64());
	m_vVideoThen = static_cast<int64>(p_opLoader -> LoadUInt64());
	return true;
}

int64 EMMediaTimer::GetSystemTimeWhenStarted() const
{
	return m_vSystemTimeWhenStarted;
}

bool EMMediaTimer::RegisterClockMaster(EMRealtimeOutputDescriptor* p_opOutputThatControlsNowTime)
{
	m_vClockMasterID = p_opOutputThatControlsNowTime -> GetID();
	return true;
}

bool EMMediaTimer::IncreaseNowFrame(int64 p_vWithFrames, int32 p_vPotentialClockMasterID)
{
	//-1 as clock master ID will force the time forward no matter source!
	if(p_vPotentialClockMasterID != m_vClockMasterID) // && m_vClockMasterID != -1)) // || m_vClockMasterID == -1 || p_vPotentialClockMasterID == -1)
		return false;
	if(m_vClockMasterID == -1)
		return false;
	if(p_vPotentialClockMasterID == -1)
		return false;

	Lock();
	if(m_eState != EM_PLAYING && m_eState != EM_RECORDING)
	{
		Unlock();
		return false;
	}

	if(p_vWithFrames >= 0)
	{
		m_vNow += p_vWithFrames;

		if(m_vIsLooped || EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingAudio() || EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingVideo())
		{
			if(m_vLoopStart != m_vLoopEnd)
			{
				if(m_vNow >= m_vLoopEnd && ! EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingAudio() && ! 
					EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingVideo())
				{
					m_vNow = m_vLoopStart;
					Unlock();
					return true;
				}
				else if(m_vNow < m_vLoopStart)
				{
					m_vNow = m_vLoopStart;
					Unlock();
					return true;
				} 
				else if(EMMediaEngine::Instance() -> GetMediaProject() -> IsPlaying() && m_vNow >= m_vLoopEnd && 
						(EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingAudio() || 
						EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingVideo()))
				{
				//	EMMediaEngine::Instance() -> GetMediaProject() ->StopVideoAudioRenderingAtOnce();
				//	int32 vCommandStop = COMMAND_STOP;
				//	EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_POST_COMMAND, &vCommandStop);
				//	Unlock();
				//	return false;
				}

			}
			else
			{
				if(m_vNow >= EMMediaEngine::Instance() -> GetMediaProject() ->GetProjectEndTimeInFrames())
				{
				//	EMMediaEngine::Instance() -> GetMediaProject() ->StopVideoAudioRenderingAtOnce();
				//	int32 vCommandStop = COMMAND_STOP;
				//	EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_POST_COMMAND, &vCommandStop);
				//	Unlock();
				}
			}
		}
	}
	Unlock();	
	return true;
}

int64 EMMediaTimer::NowFrame()
{
	Lock();
	int64 vTime = m_vNow;
	Unlock();
	return vTime;
}

int64 EMMediaTimer::NowTime()
{
	int64 vTime = 0;
	Lock();
	vTime = EMMediaUtility::Instance() -> FramesToTime(m_vNow, EMMediaUtility::Instance() -> GetSystemAudioFormat());
	Unlock();
	return vTime;
}

bool EMMediaTimer::IncreaseAudioThenFrame(int64 p_vWithFrames)
{
	Lock();
	if(p_vWithFrames >= 0 && m_eState != EM_STOPPED)
	{
		//eo << "Increasing audio then with " << (int) p_vWithFrames << " frame(s), from " << (int) m_vAudioThen << ef;
		m_vAudioThen += p_vWithFrames;
		if(m_vIsLooped || EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingAudio() || EMMediaEngine::Instance() -> 
				GetMediaProject() -> IsRenderingVideo())
		{
			if(m_vLoopStart != m_vLoopEnd)
			{
				if(m_vAudioThen >= m_vLoopEnd && ! EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingAudio() && ! 
						EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingVideo())
				{
					m_vAudioThen = m_vLoopStart;
					Unlock();
					return true;
				}
				else if(m_vNow < m_vLoopStart)
				{
					m_vAudioThen = m_vLoopStart;
					Unlock();
					return true;
				} 
				else if(m_vAudioThen > m_vLoopEnd && (EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingAudio() || 
						EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingVideo()))
				{
					Unlock();
					EMMediaEngine::Instance() -> GetMediaProject() ->StopVideoAudioRenderingAtOnce();
					int32 vCommandStop = COMMAND_STOP;
					EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_POST_COMMAND, &vCommandStop);

					//EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_STOP);
					//Do something here!
					return false;
				}
			}
			else
			{
				if(m_vAudioThen >= EMMediaEngine::Instance() -> GetMediaProject() ->GetProjectEndTimeInFrames())
				{
					EMMediaEngine::Instance() -> GetMediaProject() ->StopVideoAudioRenderingAtOnce();
					int32 vCommandStop = COMMAND_STOP;
					EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_POST_COMMAND, &vCommandStop);
					Unlock();
				}
			}
		}
	}
	Unlock();
	return true;
}

int64 EMMediaTimer::AudioThenFrame()
{
	Lock();
	int64 vTime = m_vAudioThen;
	Unlock();
	return vTime;
}

int64 EMMediaTimer::AudioThenTime()
{
	int64 vTime = 0;
	Lock();
	vTime = EMMediaUtility::Instance() -> FramesToTime(m_vAudioThen, EMMediaUtility::Instance() -> GetSystemAudioFormat());
	Unlock();
	return vTime;
}

bool EMMediaTimer::IncreaseVideoThenFrame(int64 p_vWithFrames)
{
	Lock();
	if(p_vWithFrames >= 0 && (m_eState == EM_PLAYING || m_eState == EM_RECORDING)) // || m_eState == EM_COUNTING_IN_FOR_PLAYBACK || m_eState == EM_COUNTING_IN_FOR_RECORDING))
	{
		m_vVideoThen += p_vWithFrames;
		if(m_vIsLooped || EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingAudio() || EMMediaEngine::Instance() -> 
				GetMediaProject() -> IsRenderingVideo())
		{
			if(m_vLoopStart != m_vLoopEnd)
			{
				if(m_vVideoThen >= m_vLoopEnd && ! EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingAudio() && ! 
						EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingVideo())
				{
					m_vVideoThen = m_vLoopStart;
					Unlock();
					return true;
				}
				else if(m_vNow < m_vLoopStart)
				{
					m_vVideoThen = m_vLoopStart;
					Unlock();
					return true;
				} 
				else if(m_vVideoThen > m_vLoopEnd && (EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingAudio() || 
						EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingVideo()))
				{
					EMMediaEngine::Instance() -> GetMediaProject() ->StopVideoAudioRenderingAtOnce();
					int32 vCommandStop = COMMAND_STOP;
					EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_POST_COMMAND, &vCommandStop);
					Unlock();
					//Do something here!
					return false;
				}
			}
			else
			{
				if(m_vVideoThen >= EMMediaEngine::Instance() -> GetMediaProject() ->GetProjectEndTimeInFrames())
				{
					EMMediaEngine::Instance() -> GetMediaProject() ->StopVideoAudioRenderingAtOnce();
					// Done to make TitanTV work (else exception)
					int32 vCommandStop = COMMAND_STOP;
					EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_POST_COMMAND, &vCommandStop);
					//
					Unlock();
				}
			}
		}
	}
	Unlock();
	return true;
}

int64 EMMediaTimer::VideoThenFrame()
{
	Lock();
	int64 vTime = m_vVideoThen;
	Unlock();
	return vTime;
}

int64 EMMediaTimer::VideoThenTime()
{
	int64 vTime = 0;
	Lock();
	vTime = EMMediaUtility::Instance() -> FramesToTime(m_vVideoThen, EMMediaUtility::Instance() -> GetSystemAudioFormat());
	Unlock();
	return vTime;
}

bool EMMediaTimer::IncreaseMIDIThenTime(int64 p_vWithMicroseconds)
{
	Lock();
	if(p_vWithMicroseconds >= 0 && (m_eState == EM_PLAYING || m_eState == EM_RECORDING))
	{
		m_vMIDIThen += p_vWithMicroseconds;
/*		if(m_vIsLooped || EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingAudio() || EMMediaEngine::Instance() -> 
				GetMediaProject() -> IsRenderingVideo())
		{
			if(m_vLoopStart != m_vLoopEnd)
			{
				if(m_vMIDIThen >= m_vLoopEnd && ! EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingAudio() && ! 
						EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingVideo())
				{
					m_vMIDIThen = m_vLoopStart;
					Unlock();
					return true;
				}
				else if(m_vMIDIThen > m_vLoopEnd && (EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingAudio() || 
						EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingVideo()))
				{
					Unlock();
					//Do something here!
					return false;
				}
			} 
		}*/
	}
	Unlock();
	return true;
}

int64 EMMediaTimer::MIDIThenTime()
{
	int64 vTime = 0;
	Lock();
	vTime = m_vMIDIThen;
	Unlock();
	return vTime;
}

void EMMediaTimer::SeekToFrame(int64 p_vNewFrame)
{
	Lock();
	if(p_vNewFrame < 0) 
		p_vNewFrame = 0;
	m_vNow = p_vNewFrame;
	m_vAudioThen = p_vNewFrame;
	m_vVideoThen = p_vNewFrame;
	m_vMIDIThen = EMMediaUtility::Instance() -> FramesToTime(p_vNewFrame, EMMediaUtility::Instance() -> GetSystemAudioFormat());
	Unlock();

	Notify(EM_MESSAGE_TIME_WAS_SEEKED);
}

void EMMediaTimer::Lock()
{
	m_opTimeProtectionSemaphore -> Acquire();
}

void EMMediaTimer::Unlock()
{
	m_opTimeProtectionSemaphore -> Release();
}

bool EMMediaTimer::IncreaseMetronomeNowTime(int64 p_vWithMicroseconds)
{
	m_vMetronomeThen += p_vWithMicroseconds;
	return true;
}

int64 EMMediaTimer::MetronomeNowTime()
{
	int64 vTime = 0;
	Lock();

	if(m_eState == EM_COUNTING_IN_FOR_PLAYBACK || m_eState == EM_COUNTING_IN_FOR_RECORDING)
	{
		int64 vTimePassedSinceCIStart = RealTime() - m_vSystemTimeWhenCountInStarted;
		vTime = m_vMetronomeThen + vTimePassedSinceCIStart;
		int64 vMetronomeNowFrames = EMMediaUtility::Instance() -> TimeToFrames(vTime, EMMediaUtility::Instance() -> GetSystemAudioFormat());
		if(vMetronomeNowFrames >= m_vNow-1)
		{
			Unlock();
			SetState((m_eState == EM_COUNTING_IN_FOR_PLAYBACK ? EM_PLAYING : EM_RECORDING));
		}
		else
			Unlock();
		return vTime;
	}
	else
	{
		Unlock();
		return NowTime();
	}

	Unlock();
	return vTime;
}
