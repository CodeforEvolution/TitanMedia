/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_THREAD
#define __EM_BE_THREAD

#include "EMThread.h"

#include <OS.h>

class EMThreadListener;
class EMSemaphore;

class EMBeThread : public EMThread
{
public:
	EMBeThread(const char* p_vpName, int32 p_vPriority, int32 p_vSleepTime);
	~EMBeThread();
	void Kill(bool p_vCalledFromThreadRun = false);
	static status_t ThreadEntry(void* obj);
	void Run(void);
	void SetPriority(int32 p_vPriority);
	void Sleep(int64 p_vMicroSeconds);
	void Start();
	void Suspend();
	void WaitForThread();
	
private:
	thread_id m_sThreadID;
	EMSemaphore* m_opSemaphore;
};

#endif

#endif