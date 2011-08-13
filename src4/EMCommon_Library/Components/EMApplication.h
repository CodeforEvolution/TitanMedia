/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* Handles the termination of the application
* Contains some native code
*******************************************************/

      
#include "EMGlobals.h"

#ifndef __EM_APPLICATION
#define __EM_APPLICATION

#include "EMListenerRepository.h"

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#endif

#ifdef EMCOMMON_LIBRARY_EXPORTS
#define EM_APPLICATION_LIBRARY_DECLSPECIFIER __declspec(dllexport)
#define EM_APPLICATION_LIBRARY_EXPIMP_TEMPLATE
#else
#define EM_APPLICATION_LIBRARY_DECLSPECIFIER __declspec(dllimport)
#define EM_APPLICATION_LIBRARY_EXPIMP_TEMPLATE extern "C"
#endif

class EM_APPLICATION_LIBRARY_DECLSPECIFIER EMApplication : public EMListenerRepository
{
public:
	virtual ~EMApplication();
	static const char* GetApplicationVersionNumber();
	static void InitiateQuitApplication();
	static EMApplication* Instance();
	void QuitApplication();
#ifdef PLATFORM_WINDOWS
	static void SetApplicationInstance(HINSTANCE p_opApplicationInstance);
	static HINSTANCE GetApplicationInstance();
#endif
	
protected:
	EMApplication();

private:
#ifdef PLATFORM_WINDOWS
	static HINSTANCE m_opApplicationInstance;
#endif
	static EMApplication* m_opInstance;
};

#endif

