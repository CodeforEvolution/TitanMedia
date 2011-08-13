#include "EMGlobals.h"
#include "EMSemaphore.h"

#ifdef PLATFORM_BEOS
#include "EMBeSemaphore.h"
#endif

#ifdef PLATFORM_WINDOWS
#include "EMWinSemaphore.h"
#endif

EMSemaphore::EMSemaphore()
{
}

EMSemaphore::~EMSemaphore()
{
}

EMSemaphore* EMSemaphore::CreateEMSemaphore()
{
#ifdef PLATFORM_BEOS
	return EM_new EMBeSemaphore("EMBeSemaphore");
#endif 
#ifdef PLATFORM_WINDOWS
	return EM_new EMWinSemaphore("EMWinSemaphore", 1, 1);
#endif
	return 0;
}

EMSemaphore* EMSemaphore::CreateEMSemaphore(const char* p_vName, uint32 p_vInitialCount, uint32 p_vMaxCount)
{
#ifdef PLATFORM_BEOS
	return EM_new EMBeSemaphore(p_vName);
#endif
#ifdef PLATFORM_WINDOWS
	return EM_new EMWinSemaphore(p_vName, p_vInitialCount, p_vMaxCount);
#endif
	return 0;
}

