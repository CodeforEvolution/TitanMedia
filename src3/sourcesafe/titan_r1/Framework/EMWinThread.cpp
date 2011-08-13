#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMWinThread.h"

#include "EMExceptionHandler.h"
#include "EMThreadListener.h"
#include "EMSemaphore.h"

//#include <winbase.h>

EMWinThread::EMWinThread(const char* p_vpName, int32 p_vPriority, int32 p_vSleepTime) 
	:	EMThread(p_vpName),
		m_vThreadHandle(0),
		m_vNativeThreadID(0),
		m_vIsSuspended(false),
		m_opGatewaySemaphore(NULL),
		m_opThreadProtectionSem(NULL),
		m_vWillSuspend(false)
{
	m_opGatewaySemaphore = EMSemaphore::CreateEMSemaphore(string(string(p_vpName) + string("_THREADGATEWAY")).c_str(), 0, 1);
	m_opThreadProtectionSem = EMSemaphore::CreateEMSemaphore(string(string(p_vpName) + string("_PROTECTION")).c_str(), 1, 1);

	m_vSleepTime = p_vSleepTime;
	if((m_vThreadHandle = (HANDLE) _beginthreadex(NULL, 0, ThreadEntry, this, CREATE_SUSPENDED, &m_vNativeThreadID)) == 0)
		EMDebugger("Could not create thread");
}

EMWinThread::~EMWinThread()
{
	if(m_vIsAlive)
	{
		Kill();
	}

	delete m_opThreadProtectionSem;
	delete m_opGatewaySemaphore;
	CloseHandle(m_vThreadHandle);
}

HANDLE EMWinThread::GetThreadHandle()
{
	return m_vThreadHandle;
}

unsigned int __stdcall /*void */EMWinThread::ThreadEntry(void* p_upObj)
{
	if(! p_upObj) 
		return 0;
	
	// Each thread must set the new handler individually
	// The new handler manages the reserve memory buffers
	// that are released when the system has run out of RAM

	//TODO: What to do in Windows? How does this work there?
	//set_new_handler(&(EMExceptionHandler::NewHandler));

	((EMWinThread*) p_upObj) -> Run();
	return 0;
}

void EMWinThread::Run(void)
{
	//Keep calling listener for as long as that flag is set!
	m_opThreadProtectionSem -> Acquire();
	NotifyCreated();

	while(true)
	{
		if(! m_vIsAlive)
			break;
		m_opGatewaySemaphore -> Acquire();
		if(! m_vIsAlive)
			break;
		Notify();
		if(! m_vWillSuspend)
			m_opGatewaySemaphore -> Release();
		else
			m_vWillSuspend = false;

		Sleep(m_vSleepTime);
	}

	NotifyKilled();
	m_opThreadProtectionSem -> Release();
}


void EMWinThread::Kill(bool p_vCalledFromThreadRun)
{
	m_vIsAlive = false;
	// If called from thread run, these calls to the semaphore
	// would lock the thread = not good
	if(! p_vCalledFromThreadRun)
	{
		m_opGatewaySemaphore -> Release();
	}
	m_opThreadProtectionSem -> Acquire();
	m_opThreadProtectionSem -> Release();
}

void EMWinThread::Start()
{
	m_vIsAlive = true;
	m_opGatewaySemaphore -> Release();
	int32 xxx = ResumeThread(m_vThreadHandle);
}

void EMWinThread::StartSuspended()
{
	m_vIsAlive = true;
	m_vIsSuspended = true;
	int32 xxx = ResumeThread(m_vThreadHandle);
}

void EMWinThread::Suspend(bool p_vCalledFromThreadRun)
{
	if(! m_vIsSuspended)
	{
		m_vIsSuspended = true;
		if(! p_vCalledFromThreadRun)
			m_opGatewaySemaphore -> Acquire();
		else
			m_vWillSuspend = true;
	}
}

void EMWinThread::Resume()
{
	//Assume that this call can never be made from within the own thread!
	if(m_vIsSuspended)
	{	
		m_vWillSuspend = false;
		m_vIsSuspended = false;
		m_opGatewaySemaphore -> Release();
	}
}

void EMWinThread::SetPriority(int32 p_vPriority)
{
//	m_opThread -> SetThreadPriority(p_vPriority);
	uint32 vClass = IDLE_PRIORITY_CLASS;
	uint32 vPrio = THREAD_PRIORITY_IDLE;

	if(p_vPriority < 5 && p_vPriority > 0)
	{
		vClass = IDLE_PRIORITY_CLASS;
		vPrio = THREAD_PRIORITY_LOWEST;
	}
	else if(p_vPriority < 15)
	{
		vClass = EM_THREAD_IDLE_PRIORITY;
		vPrio = THREAD_PRIORITY_NORMAL;
	}
	else if(p_vPriority < 50)
	{
		vClass = NORMAL_PRIORITY_CLASS;
		vPrio = THREAD_PRIORITY_NORMAL;
	}
	else if(p_vPriority < 110)
	{
		vClass = HIGH_PRIORITY_CLASS;
		vPrio = THREAD_PRIORITY_NORMAL;
	}
	else if(p_vPriority >= 110)
	{
		vClass = REALTIME_PRIORITY_CLASS;
		vPrio = THREAD_PRIORITY_TIME_CRITICAL;
	}
	
	SetPriorityClass(m_vThreadHandle, vClass);
	SetThreadPriority(m_vThreadHandle, p_vPriority);
}

void EMWinThread::Sleep(int64 p_vMicroSeconds)
{
	//TODO: Find out how to sleep!
	//????Wait(p_vMicroSeconds);
	::Sleep(p_vMicroSeconds / 1000.0);
}

void EMWinThread::WaitForThread()
{
}

#endif

