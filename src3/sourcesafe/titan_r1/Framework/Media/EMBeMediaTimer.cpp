#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#include "EMBeMediaTimer.h"
#include "EMBeMediaUtility.h"
#include "EMMediaIDManager.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMCommandRepository.h"
#include "CommandIDs.h"

#include <MediaKit.h>

EMBeMediaTimer::EMBeMediaTimer() 
	:	EMMediaTimer()
{
	EMBeMediaUtility::push(this, "EMBeMediaTimer");
	m_vTimeProtectionSemaphore = create_sem(1, "Titan time protection sem");
	m_vSem = create_sem(1, "Other time protection semaphore");
	m_vID = EMMediaIDManager::MakeID();
	m_vVideoThen = 0;
}

EMBeMediaTimer::~EMBeMediaTimer()
{
	EMBeMediaUtility::pop("EMBeMediaTimer");

	delete_sem(m_vTimeProtectionSemaphore);
}

bool EMBeMediaTimer::IncreaseNowFrame(int64 p_vWithFrames)
{
//	status_t vAcquireResult = acquire_sem(m_vTimeProtectionSemaphore);
//	if(vAcquireResult != B_NO_ERROR)
//		EMDebugger("ERROR! EMBeMediaTimer could not acquire semaphore for timer protection!");
	
	if(! m_vIsStarted)
		return false;

	if(p_vWithFrames >= 0)
	{
		m_vNow += p_vWithFrames;

		if(m_vIsLooped || EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingAudio() || EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingVideo())
		{
			if(m_vLoopStart != m_vLoopEnd)
			{
				if(m_vNow >= m_vLoopEnd && ! EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingAudio() && ! EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingVideo())
				{
					m_vNow = m_vLoopStart;
//					release_sem(m_vTimeProtectionSemaphore);
					SeekToFrame(m_vLoopStart);
					return true;
				}
 				else if(EMMediaEngine::Instance() -> GetMediaProject() -> IsPlaying() && m_vNow >= m_vLoopEnd && 
						(EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingAudio() || 
						EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingVideo()))
				{
					EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_STOP);
					return false;
				}
			}
		}
	}
	else
		EMDebugger("ERROR in MediaTimer::IncreaseNowFrame. Cannot increase with negative number of frames!");
//	release_sem(m_vTimeProtectionSemaphore);
	return true;
}

int64 EMBeMediaTimer::NowFrame() const
{
//	status_t vAcquireResult = acquire_sem(m_vTimeProtectionSemaphore);
//	if(vAcquireResult != B_NO_ERROR)
//		EMDebugger("ERROR! EMBeMediaTimer could not acquire semaphore for timer protection!");

	int64 vTime = m_vNow;
//	release_sem(m_vTimeProtectionSemaphore);
	return vTime;
}

int64 EMBeMediaTimer::NowTime()
{
//	status_t vAcquireResult = acquire_sem(m_vTimeProtectionSemaphore);
//	if(vAcquireResult != B_NO_ERROR)
//		EMDebugger("ERROR! EMBeMediaTimer could not acquire semaphore for timer protection!");

	int64 vTime = EMBeMediaUtility::FramesToTime(m_vNow, EMBeMediaUtility::GetSystemAudioFormat());
//	release_sem(m_vTimeProtectionSemaphore);
	return vTime;
}

bool EMBeMediaTimer::IncreaseAudioThenFrame(int64 p_vWithFrames)
{
//	status_t vAcquireResult = acquire_sem(m_vTimeProtectionSemaphore);
//	if(vAcquireResult != B_NO_ERROR)
//		EMDebugger("ERROR! EMBeMediaTimer could not acquire semaphore for timer protection!");

	if(p_vWithFrames >= 0)
	{
		m_vAudioThen += p_vWithFrames;
		if(m_vIsLooped || EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingAudio() || EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingVideo())
		{
			if(m_vLoopStart != m_vLoopEnd)
			{
				if(m_vAudioThen >= m_vLoopEnd && ! EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingAudio() && ! EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingVideo())
				{
					m_vAudioThen = m_vLoopStart;
//					release_sem(m_vTimeProtectionSemaphore);
					return true;
				}
				else if(m_vAudioThen > m_vLoopEnd && (EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingAudio() || EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingVideo()))
				{
//					release_sem(m_vTimeProtectionSemaphore);
					//Do something here!
					return false;
				}
			}
		}
	}
	else
		EMDebugger("ERROR in MediaTimer::IncreaseAudioThenFrame. Cannot increase with negative number of frames!");
	return true;
//	release_sem(m_vTimeProtectionSemaphore);
}

int64 EMBeMediaTimer::AudioThenFrame() const
{
//	status_t vAcquireResult = acquire_sem(m_vTimeProtectionSemaphore);
//	if(vAcquireResult != B_NO_ERROR)
//		EMDebugger("ERROR! EMBeMediaTimer could not acquire semaphore for timer protection!");

	int64 vTime = m_vAudioThen;
	release_sem(m_vTimeProtectionSemaphore);
	return vTime;
}

int64 EMBeMediaTimer::AudioThenTime()
{
//	status_t vAcquireResult = acquire_sem(m_vTimeProtectionSemaphore);
//	if(vAcquireResult != B_NO_ERROR)
//		EMDebugger("ERROR! EMBeMediaTimer could not acquire semaphore for timer protection!");

	int64 vTime = EMBeMediaUtility::FramesToTime(m_vAudioThen, EMBeMediaUtility::GetSystemAudioFormat());
//	release_sem(m_vTimeProtectionSemaphore);
	return vTime;
}

bool EMBeMediaTimer::IncreaseVideoThenFrame(int64 p_vWithFrames)
{
//	status_t vAcquireResult = acquire_sem(m_vTimeProtectionSemaphore);
//	if(vAcquireResult != B_NO_ERROR)
//		EMDebugger("ERROR! EMBeMediaTimer could not acquire semaphore for timer protection!");

	if(p_vWithFrames >= 0)
	{
		m_vVideoThen += p_vWithFrames;
		if(m_vIsLooped || EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingAudio() || EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingVideo())
		{
			if(m_vLoopStart != m_vLoopEnd)
			{
				if(m_vVideoThen >= m_vLoopEnd && ! EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingAudio() && ! EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingVideo())
				{
					m_vVideoThen = m_vLoopStart;
//					release_sem(m_vTimeProtectionSemaphore);
					return true;
				}
				else if(m_vVideoThen > m_vLoopEnd && (EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingAudio() || EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingVideo()))
				{
//					release_sem(m_vTimeProtectionSemaphore);
					//Do something here!
					return false;
				}
			}
		}
	}
	else
		EMDebugger("ERROR in MediaTimer::IncreaseAudioThenFrame. Cannot increase with negative number of frames!");
//	release_sem(m_vTimeProtectionSemaphore);
	return true;
}

int64 EMBeMediaTimer::VideoThenFrame() const
{
//	status_t vAcquireResult = acquire_sem(m_vTimeProtectionSemaphore);
//	if(vAcquireResult != B_NO_ERROR)
//		EMDebugger("ERROR! EMBeMediaTimer could not acquire semaphore for timer protection!");
	int64 vTime = m_vVideoThen;
//	release_sem(m_vTimeProtectionSemaphore);
	return vTime;
}

int64 EMBeMediaTimer::VideoThenTime()
{
//	status_t vAcquireResult = acquire_sem(m_vTimeProtectionSemaphore);
//	if(vAcquireResult != B_NO_ERROR)
//		EMDebugger("ERROR! EMBeMediaTimer could not acquire semaphore for timer protection!");

	int64 vTime = EMBeMediaUtility::FramesToTime(m_vVideoThen, EMBeMediaUtility::GetSystemAudioFormat());
//	release_sem(m_vTimeProtectionSemaphore);
	return vTime;
}

void EMBeMediaTimer::SeekToFrame(int64 p_vNewFrame)
{
	Lock();
	
	if(m_vIsStarted)
		Notify(EM_MESSAGE_PAUSE);
		
	status_t vAcquireResult = acquire_sem(m_vTimeProtectionSemaphore);
	if(vAcquireResult != B_NO_ERROR)
		EMDebugger("ERROR! EMBeMediaTimer could not acquire semaphore for timer protection!");

	if(p_vNewFrame < 0) 
		p_vNewFrame = 0;
	
	m_vNow = p_vNewFrame;
	m_vAudioThen = p_vNewFrame;
	m_vVideoThen = p_vNewFrame;
	release_sem(m_vTimeProtectionSemaphore);
	Notify(EM_MESSAGE_TIME_SEEKED);

	if(m_vIsStarted)
		Notify(EM_MESSAGE_RESUME);

	Unlock();
}

void EMBeMediaTimer::Lock()
{
//	status_t vAcquireResult = acquire_sem(m_vSem);
//	if(vAcquireResult != B_NO_ERROR)
//		EMDebugger("ERROR! EMBeMediaTimer could not acquire semaphore for timer protection!");
}

void EMBeMediaTimer::Unlock()
{
//	release_sem(m_vSem);
}

int64 EMBeMediaTimer::RealTime() const
{
	return system_time();
}

bool EMBeMediaTimer::ClearData()
{
	return EMMediaTimer::ClearData();
}

#endif
