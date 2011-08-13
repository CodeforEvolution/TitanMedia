/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_EXCEPTION_HANDLER
#define __EM_EXCEPTION_HANDLER

#include "EMException.h"

class __declspec(dllexport) EMExceptionHandler
{
public:
	~EMExceptionHandler();
	static void DeleteInstance();
	void HandleException(EMException p_oException);
	static EMExceptionHandler* Instance();
	static void NewHandler();
	
protected:
	EMExceptionHandler();
	
private:
	static char* m_vpBuffer;
	static EMExceptionHandler* m_opInstance;
};

#endif


