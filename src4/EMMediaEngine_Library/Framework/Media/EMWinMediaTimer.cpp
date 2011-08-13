#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMMediaGlobals.h"
#include "EMWinMediaTimer.h"
//#include "EMWinMediaUtility.h"
#include "EMMediaIDManager.h"
//#include "EMMediaEngine.h"
//#include "EMMediaProject.h"
//#include "EMCommandRepository.h"
//#include "EMSemaphore.h"
//#include "CommandIDs.h"
#include "EMWinMIDISystem.h"
//#include "EMSettingsRepository.h"
//#include "EMSettingIDs.h"
//#include "EMMediaMetronome.h"

#include <objbase.h>
#include <uuids.h>

EMWinMediaTimer::EMWinMediaTimer() 
	:	EMMediaTimer(),
		m_opSystemReferenceClock(NULL)
{
	HRESULT vResult = CoCreateInstance(CLSID_SystemClock, NULL, CLSCTX_INPROC, IID_IReferenceClock, (void**) &m_opSystemReferenceClock);
	if(FAILED(vResult) || m_opSystemReferenceClock == NULL)
		eo << "Failed to get system reference clock!" << ef;
}

EMWinMediaTimer::~EMWinMediaTimer()
{
	if(m_opSystemReferenceClock != NULL)
		int v = m_opSystemReferenceClock -> Release();
	m_opSystemReferenceClock = NULL;
}

void EMWinMediaTimer::SetNativeReferenceClock(void* p_upRefClock)
{
	eo << "NOT IMPLEMENTED" << ef;
}

void* EMWinMediaTimer::GetNativeReferenceClock() const
{
	if(m_opSystemReferenceClock == NULL)
		eo << "Failed to get system reference clock!" << ef;
	return static_cast<void*>(m_opSystemReferenceClock);
}

int64 EMWinMediaTimer::RealTime() const
{
	if(m_opSystemReferenceClock != NULL)
	{
		REFERENCE_TIME sReferenceTimeStruct = 0;
		m_opSystemReferenceClock -> GetTime(&sReferenceTimeStruct);
		return static_cast<int64>(static_cast<int64>(sReferenceTimeStruct) / 10.0);
	}
	else
	{
		eo << "ERROR! No system reference clock set! Is MIDI disabled? Is audio disabled? Must have an IReferenceClock!" << ef;
		return 0;
	}
}

void EMWinMediaTimer::OnStartPlayback()
{
	if(m_opSystemReferenceClock != NULL) 
		m_vSystemTimeWhenStarted = RealTime();
	else 
		m_vSystemTimeWhenStarted = 0;
}

void EMWinMediaTimer::OnStopPlayback()
{
	m_vSystemTimeWhenStarted = 0;
}

void EMWinMediaTimer::OnStartRecording()
{
	if(m_opSystemReferenceClock != NULL) 
		m_vSystemTimeWhenStarted = RealTime();
	else 
		m_vSystemTimeWhenStarted = 0;
}

void EMWinMediaTimer::OnStopRecording()
{
	m_vSystemTimeWhenStarted = 0;
}

void EMWinMediaTimer::OnCountInPlayback()
{
}

void EMWinMediaTimer::OnCountInRecording()
{
}

bool EMWinMediaTimer::ClearData()
{
	return EMMediaTimer::ClearData();
}

#endif
