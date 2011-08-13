#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#include "EMBeSemaphore.h"

#include <OS.h>

EMBeSemaphore::EMBeSemaphore(char* p_vName)
{
	m_vSemaphore = create_sem(1, p_vName);
	m_oSemaphoreName = p_vName;
}

EMBeSemaphore::~EMBeSemaphore()
{
	delete_sem(m_vSemaphore);
}

void EMBeSemaphore::Acquire()
{
	acquire_sem(m_vSemaphore);
}

void EMBeSemaphore::Release()
{
	release_sem(m_vSemaphore);
}

#endif