/*******************************************************
* Creator: Richard Kronfält
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_THREAD
#define __EM_WIN_THREAD

#include "EMThread.h"

#include <windows.h>
#include <process.h>

class EMThreadListener;
class EMSemaphore;

class __declspec(dllexport) EMWinThread : public EMThread
{
public:
	EMWinThread(const char* p_vpName, int32 p_vPriority, int32 p_vSleepTime);
	~EMWinThread();
	HANDLE GetThreadHandle();
//	static uint32 ThreadEntry(void* lpParameter);
//	static UINT ThreadEntry(void* p_upObj);
	static unsigned int __stdcall ThreadEntry(void* p_upObj);
	void Run(void);
	
	void Kill(bool p_vCalledFromThreadRun = false);
	void Start();
	void StartSuspended();
	void Suspend(bool p_vCalledFromThreadRun = false);
	void Resume();
	
	void SetPriority(int32 p_vPriority);
	void Sleep(int64 p_vMicroSeconds);
	void WaitForThread();
	
private:
	HANDLE m_vThreadHandle;
	unsigned int m_vNativeThreadID;
	bool m_vIsSuspended;
	EMSemaphore* m_opThreadProtectionSem;
	EMSemaphore* m_opGatewaySemaphore;

	bool m_vWillSuspend;
};

#endif

#endif