/*******************************************************
* Creator: Richard Kronfält
* Component: none
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_SEMAPHORE
#define __EM_SEMAPHORE

class __declspec(dllexport) EMSemaphore
{
public:
	virtual ~EMSemaphore();
	virtual bool Acquire() = 0;
	virtual bool NonBlockingAcquire() = 0;
	static EMSemaphore* CreateEMSemaphore();
	static EMSemaphore* CreateEMSemaphore(const char* p_vName, uint32 p_vInitialCount = 1, uint32 p_vMaxCount = 1);
	virtual void Release() = 0;
	virtual string GetSemaphoreName() = 0;

protected:
	EMSemaphore();
};

#endif