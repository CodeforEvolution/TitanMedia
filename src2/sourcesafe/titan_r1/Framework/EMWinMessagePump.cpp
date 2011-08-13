#include "EMWinMessagePump.h"

#ifdef PLATFORM_WINDOWS

#include "EMSemaphore.h"
#include "EMThread.h"

const char* EM_SEM_MP_LOCK_THREAD_NAME = "MP_LockThreadSem";
const char* EM_SEM_MP_PROTECT_DATA_NAME = "MP_ProtectDataSem";
const char* EM_MP_THREAD_NAME = "MessagePump_Thread";

EMWinMessagePump* EMWinMessagePump::m_opInstance = NULL;

EMWinMessagePump::EMWinMessagePump() :
m_opLockThreadSemaphore(EMSemaphore::CreateEMSemaphore(EM_SEM_MP_LOCK_THREAD_NAME, 0, 1000)),
m_opProtectDataSemaphore(EMSemaphore::CreateEMSemaphore(EM_SEM_MP_PROTECT_DATA_NAME, 1, 1)),
m_opThread(EMThread::CreateEMThread(EM_MP_THREAD_NAME))
{
	m_opThread -> AddListener(this);
	m_opThread -> Start();
}

EMWinMessagePump::~EMWinMessagePump()
{
	delete m_opThread;
	delete m_opLockThreadSemaphore;
	delete m_opProtectDataSemaphore;
}

void EMWinMessagePump::AddMessage(HWND p_opWindowHandle, uint16 p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo)
{
	m_opProtectDataSemaphore -> Acquire();
	m_oMessageQueue.push_back(EMWinMessage(p_opWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo));
	m_opProtectDataSemaphore -> Release();
	m_opLockThreadSemaphore -> Release();
}

EMWinMessagePump* EMWinMessagePump::Instance()
{
	if(m_opInstance == NULL)
		m_opInstance = EM_new EMWinMessagePump();
	return m_opInstance;
}

void EMWinMessagePump::ThreadRun(EMThread* p_opThread)
{
	m_opLockThreadSemaphore -> Acquire();
	m_opProtectDataSemaphore -> Acquire();
	m_oMessageThreadCopy = m_oMessageQueue.front();
	m_oMessageQueue.pop_front();
	m_opProtectDataSemaphore -> Release();
	PostMessage(m_oMessageThreadCopy.m_opWindowHandle, m_oMessageThreadCopy.m_vMessage, m_oMessageThreadCopy.m_vParameterOne, m_oMessageThreadCopy.m_vParameterTwo);
}

#endif