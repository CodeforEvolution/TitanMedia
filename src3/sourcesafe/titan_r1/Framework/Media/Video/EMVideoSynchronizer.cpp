#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS
#include "EMVideoSynchronizer.h"
#include "EMSemaphore.h"

#include "EMMediaEngine.h"
#include "EMSettingsRepository.h" //For collecting the framerate
#include "EMSettingIDs.h"

EMVideoSynchronizer* EMVideoSynchronizer::m_opInstance = NULL;

EMVideoSynchronizer::EMVideoSynchronizer()
{
	HRESULT hr = CoCreateInstance(CLSID_SystemClock, NULL, CLSCTX_INPROC, IID_IReferenceClock, (void**)&m_opIClock);
	m_opRealTimeSynchronizerClockSemaphore = EMSemaphore::CreateEMSemaphore("EMVideoSynchronizerSemaphore");
	m_opIClock -> GetTime(&m_vRealTimeClockStartTime);
	m_oTimerEvent = CreateEvent(NULL, false, false, NULL);
	m_vOffsetTime = 0;
	m_vNeedsToSynch = false;
	m_vPassedRealTimeClockTime = 0;
}

EMVideoSynchronizer::~EMVideoSynchronizer()
{
	if(m_opIClock != NULL)
	{
		m_opIClock -> Release();
		m_opIClock = NULL;
	}

	CloseHandle(m_oTimerEvent);
	delete m_opRealTimeSynchronizerClockSemaphore;
}

void EMVideoSynchronizer::Delete()
{
	if(m_opInstance != NULL)
		delete m_opInstance;

	m_opInstance = NULL;
}

EMVideoSynchronizer* EMVideoSynchronizer::Instance()
{
	if(m_opInstance != NULL)
	{
		return m_opInstance;		
	} else if(m_opInstance == NULL)
		m_opInstance = EM_new EMVideoSynchronizer();

	return m_opInstance;
}

void EMVideoSynchronizer::SetRelativeOffsetTime(int64 p_vOffsetTime)
{
	m_opRealTimeSynchronizerClockSemaphore -> Acquire();

	m_vOffsetTime = p_vOffsetTime;

	m_opRealTimeSynchronizerClockSemaphore -> Release();
}

int64 EMVideoSynchronizer::GetRelativeOffsetTime()
{
	return m_vOffsetTime;
}

void EMVideoSynchronizer::SignalForSeek()
{
	SetEvent(m_oTimerEvent);
}

bool EMVideoSynchronizer::WaitForBufferStart(int64 p_vBufferRelativeStart)
{
	m_opRealTimeSynchronizerClockSemaphore -> Acquire();

	int64 vCurrentRelatimeClockTime;

	//Get the current clock time
	m_opIClock -> GetTime(&vCurrentRelatimeClockTime);

	//Get the time that the clock has to wait (25.0 is static for test purposes)

	int64 vWaitingTime = static_cast<int64>((10000000.0 / *(static_cast<float*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FRAMERATE)))) - (vCurrentRelatimeClockTime - m_vPassedRealTimeClockTime));

	//The buffer time has already passed, no time to wait!
	if(vWaitingTime < 0 || m_vPassedRealTimeClockTime == 0)
	{
		m_opIClock -> GetTime(&m_vPassedRealTimeClockTime);
		m_opRealTimeSynchronizerClockSemaphore -> Release();
		return false;
	}

	//We should wait a while for the buffer time to occur
	ResetEvent(m_oTimerEvent);
	HRESULT hr = m_opIClock -> AdviseTime(vCurrentRelatimeClockTime, vWaitingTime, (unsigned long)m_oTimerEvent, &hEventId);
	WaitForSingleObject(m_oTimerEvent, INFINITE);

	m_opIClock -> GetTime(&m_vPassedRealTimeClockTime);

	m_opRealTimeSynchronizerClockSemaphore -> Release();
	return true;
}

/*
bool EMVideoSynchronizer::WaitForBufferStart(int64 p_vBufferRelativeStart)
{
	if(p_vBufferRelativeStart == 143000000)
	{
		int arne = 1;
	}
	m_opRealTimeSynchronizerClockSemaphore -> Acquire();

	int64 vCurrentRelatimeClockTime;

	//Get the current clock time
	m_opIClock -> GetTime(&vCurrentRelatimeClockTime);

	//Get the relative clock time
	m_vCurrentRelativeTime = vCurrentRelatimeClockTime - m_vRealTimeClockStartTime;

	//Get the time that the clock has to wait
	int64 vWaitingTime = 10*p_vBufferRelativeStart - (m_vCurrentRelativeTime + 10*m_vOffsetTime);

	//The buffer time has already passed, no time to wait!
	if(vWaitingTime < 0)
	{
		m_opRealTimeSynchronizerClockSemaphore -> Release();
		return false;
	}

	//We should wait a while for the buffer time to occur
	ResetEvent(m_oTimerEvent);
	HRESULT hr = m_opIClock -> AdviseTime(vCurrentRelatimeClockTime, vWaitingTime, (unsigned long)m_oTimerEvent, &hEventId);
	WaitForSingleObject(m_oTimerEvent, INFINITE);

	m_opRealTimeSynchronizerClockSemaphore -> Release();
	return true;
}*/

void EMVideoSynchronizer::SetNeedsToSynch(bool p_vNeedsToSynch)
{
	m_vNeedsToSynch = p_vNeedsToSynch;
}

int64 EMVideoSynchronizer::GetSynchClockRelativeRealTime()
{
	int64 vCurrentRelatimeClockTime;
	//Get the current clock time
	m_opIClock -> GetTime(&vCurrentRelatimeClockTime);
	//Return the relative clock time
	return ((vCurrentRelatimeClockTime - m_vRealTimeClockStartTime) / 10);
}

bool EMVideoSynchronizer::NeedsToSynch()
{
	return m_vNeedsToSynch;
}

void EMVideoSynchronizer::ZeroRealTimeClock()
{
	m_opRealTimeSynchronizerClockSemaphore -> Acquire();

	m_opIClock -> GetTime(&m_vRealTimeClockStartTime);

	m_opRealTimeSynchronizerClockSemaphore -> Release();

}


#endif