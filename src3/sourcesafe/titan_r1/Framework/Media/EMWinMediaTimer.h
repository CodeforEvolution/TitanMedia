/*******************************************************
* Creator: Richard Kronfalt
* Portability: Native
*-------------------------------------------------------
* 
* This class keeps track of the time.
*
* It introduces two new concepts of time:
* Now-time and Then-time. The first is simply where the
* heck the position-pointer is (or should be) in the song,
* (it's controlled by the master-output) while Then-time is
* where the processing is.
*
* Then-time should therefor (hopefully :)) always be ahead
* of Now-time, since the processing should be done somewhat
* in advance.
*
* The difference in time between these to times is 
* a result of a buffer in the the output nodes (I think :-))
* namely the event-queue in BTimedEventQueue, which the
* EMBeOutputNode uses for storing incoming BBuffers.
*
* Richard Kronfält, 00-11-28
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_MEDIA_TIMER
#define __EM_WIN_MEDIA_TIMER

#include "EMMediaTimer.h"
#include "EMListenerRepository.h"
#include "EMMediaFormat.h"

class EMSemaphore;

class EMWinMediaTimer : public EMMediaTimer
{
public:
	EMWinMediaTimer();
	bool ClearData();
	void OnStartPlayback();
	void OnStopPlayback();
	void OnStartRecording();
	void OnStopRecording();
	void OnCountInPlayback();
	void OnCountInRecording();
	int64 RealTime() const;

	void SetNativeReferenceClock(void* p_upRefClock);
	void* GetNativeReferenceClock() const;

protected:
	~EMWinMediaTimer();

private:
	IReferenceClock* m_opSystemReferenceClock;
};

#endif

#endif
