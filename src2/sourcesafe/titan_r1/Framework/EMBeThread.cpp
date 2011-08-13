#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#include "EMBeThread.h"

#include "EMExceptionHandler.h"
#include "EMThreadListener.h"
#include "EMSemaphore.h"

EMBeThread::EMBeThread(const char* p_vpName, int32 p_vPriority, int32 p_vSleepTime) : 
EMThread(p_vpName)
{
	m_opSemaphore = EMSemaphore::CreateSemaphore();
	m_vSleepTime = p_vSleepTime;
	m_sThreadID = spawn_thread(ThreadEntry, p_vpName, p_vPriority, this);
	
}

EMBeThread::~EMBeThread()
{
	if(m_vIsAlive)
	{
		Kill();
		WaitForThread();
	}
}

void EMBeThread::Kill(bool p_vCalledFromThreadRun)
{
	m_vIsAlive = false;
	m_vShouldRun = false;
	// If called from thread run, these calls to the semaphore
	// would lock the thread = not good
	if(!p_vCalledFromThreadRun)
	{
		m_opSemaphore -> Acquire();
		m_opSemaphore -> Release();
	}
}

status_t EMBeThread::ThreadEntry(void* obj)
{
	if(!obj) 
		return B_ERROR;
	
	// Each thread must set the new handler individually
	// The new handler manages the reserve memory buffers
	// that are released when the system has run out of RAM
	set_new_handler(&(EMExceptionHandler::NewHandler));

	((EMBeThread*) obj)	-> Run();
	return B_OK;
}

void EMBeThread::Run(void)
{
	//Keep calling listener for as long as that flag is set!
	while(m_vIsAlive)
	{
		if(m_vShouldRun)
		{
			m_opSemaphore -> Acquire();
			Notify();
			m_opSemaphore -> Release();
		}
			
		Sleep(m_vSleepTime);
	}
}

void EMBeThread::SetPriority(int32 p_vPriority)
{
	set_thread_priority(m_sThreadID, p_vPriority);
}

void EMBeThread::Sleep(int64 p_vMicroSeconds)
{
	snooze(p_vMicroSeconds);
}

void EMBeThread::Start()
{
	m_vIsAlive = true;
	m_vShouldRun = true;
	resume_thread(m_sThreadID);
}

void EMBeThread::Suspend()
{
	m_vShouldRun = false;
	m_opSemaphore -> Acquire();
	m_opSemaphore -> Release();
}

void EMBeThread::WaitForThread()
{
	status_t vErrorCode;// = 0xFFFF; // Should be a safe value
//	while(wait_for_thread( m_sThreadID, &vErrorCode ) == B_INTERRUPTED);
//	int vIndex = 0; // Debug thingie
//	while(vErrorCode != B_OK)// && vErrorCode != B_INTERRUPTED)
//	{
//		vIndex++;
		while(wait_for_thread(m_sThreadID, &vErrorCode) == B_INTERRUPTED)
			;
//		if(vIndex == 100)
//			EMDebug("Waiting for a thread to die");
//	}
		
}

#endif

