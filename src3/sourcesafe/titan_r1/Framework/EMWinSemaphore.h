/*******************************************************
* Creator: Richard Kronfält
* Component: none
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_SEMAPHORE
#define __EM_WIN_SEMAPHORE

#include "EMSemaphore.h"

#include <windows.h>

class __declspec(dllexport) EMWinSemaphore : public EMSemaphore
{
public:
	EMWinSemaphore(const char* p_vName, uint32 p_vInitialCount = 1, uint32 p_vMaxCount = 1);
	~EMWinSemaphore();
	bool Acquire();
	bool NonBlockingAcquire();
	void Release();
	string GetSemaphoreName();

private:
	HANDLE m_vSemaphore;
	string m_oSemaphoreName;
};

#endif

#endif