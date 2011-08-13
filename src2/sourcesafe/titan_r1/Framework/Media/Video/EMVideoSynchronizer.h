/*******************************************************
* Creator: Jesper Svensson
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_VIDEO_SYNCHRONIZER
#define __EM_VIDEO_SYNCHRONIZER

#ifdef PLATFORM_WINDOWS

#include <dshow.h>
class EMSemaphore;

class EMVideoSynchronizer
{

public:
	~EMVideoSynchronizer();
	EMVideoSynchronizer();
	static EMVideoSynchronizer* Instance();
	static void Delete();
	void SetRelativeOffsetTime(int64 p_vOffsetTime);
	int64 GetRelativeOffsetTime();
	void SignalForSeek();
	bool WaitForBufferStart(int64 p_vBufferStart);
	void ZeroRealTimeClock();
	void SetNeedsToSynch(bool p_vNeedsToSynch);
	bool NeedsToSynch();
	int64 GetSynchClockRelativeRealTime();
private:
	static EMVideoSynchronizer* m_opInstance;
	IReferenceClock* m_opIClock;
	EMSemaphore* m_opRealTimeSynchronizerClockSemaphore;
	HANDLE m_oTimerEvent;
	DWORD hEventId;
	int64 m_vRealTimeClockStartTime;
	int64 m_vOffsetTime;
	bool m_vNeedsToSynch;
	int64 m_vCurrentRelativeTime;

	//New approach
	int64 m_vPassedRealTimeClockTime;
};

#endif
#endif