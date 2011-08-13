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

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_MEDIA_TIMER
#define __EM_BE_MEDIA_TIMER

#include "EMMediaTimer.h"
#include "EMListenerRepository.h"
#include "EMMediaFormat.h"

class EMBeMediaTimer : public EMMediaTimer
{
public:
	EMBeMediaTimer(); //TODO: Make private/protected. or make static "Instance" method (even tho parent has one already)
	//Controls both Now and Then. If seeked, then we have to reset both, you see! (BIIAAATCH!)
	void SeekToFrame(int64 p_vNewFrame);
	
	//Actual song time
	bool IncreaseNowFrame(int64 p_vWithFrames);
	int64 NowFrame() const;
	int64 NowTime();
	int64 RealTime() const;
	
	void Lock();
	void Unlock();
	
	//The processing time (hopefully > or >> song time)
	bool IncreaseAudioThenFrame(int64 p_vWithFrames);
	int64 AudioThenFrame() const;
	int64 AudioThenTime();

	bool IncreaseVideoThenFrame(int64 p_vWithFrames);
	int64 VideoThenFrame() const;
	int64 VideoThenTime();
	
	static status_t ThreadEntry(void* p_upObj);
	void Run();

	bool ClearData();

protected:
	~EMBeMediaTimer();

private:
	sem_id m_vTimeProtectionSemaphore;
	sem_id m_vSem;
};

#endif

#endif
