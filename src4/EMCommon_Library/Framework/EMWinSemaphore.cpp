#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMWinSemaphore.h"

EMWinSemaphore::EMWinSemaphore(const char* p_vName, uint32 p_vInitialCount, uint32 p_vMaxCount)
{
	m_vSemaphore = CreateSemaphore(NULL, p_vInitialCount, p_vMaxCount, p_vName);
	if(p_vName != NULL)
		m_oSemaphoreName = string(p_vName);
	else
		m_oSemaphoreName = "Noname";
}

EMWinSemaphore::~EMWinSemaphore()
{
	if(! CloseHandle(m_vSemaphore))
		return; //TODO: Throw exception?
}

bool EMWinSemaphore::Acquire()
{
	try
	{
		DWORD vError = WaitForSingleObject(m_vSemaphore, INFINITE);
		if(vError != WAIT_OBJECT_0) // || vError == )
		{
			eo << "WARNING! Semaphore acquire failed!" << ef;
			return false; //TODO: Throw exception?+
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception while acquiring semaphore (1)!");
	}
	return true;
}

bool EMWinSemaphore::NonBlockingAcquire()
{
	try
	{
		DWORD vError = WaitForMultipleObjects(1, &m_vSemaphore, TRUE, 10);
		if(vError == WAIT_OBJECT_0)
			return true;
		else if(vError == WAIT_ABANDONED_0)
			return false;
		else if(vError == WAIT_TIMEOUT)
			return false;
		else
			return false;
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception while acquiring semaphore (2)!");
	}
	return false;
}

void EMWinSemaphore::Release()
{
	try
	{
		//eo << "Trying to release semaphore: " << m_oSemaphoreName << ef;
		if( ! ReleaseSemaphore(m_vSemaphore, 1, NULL))
		{
		//eo << "Release semaphore failed: " << m_oSemaphoreName << ef;
			return; //TODO: Throw exception?
		}
		//eo << "Release semaphore successful: " << m_oSemaphoreName << ef;
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception while releasing semaphore (1)!");
	}
}

string EMWinSemaphore::GetSemaphoreName()
{
	return m_oSemaphoreName;
}

#endif