#ifndef __EM_MEDIA_DEBUG_LOG
#define __EM_MEDIA_DEBUG_LOG

#include "EMGlobals.h"

//#include <fstream>
#include <windows.h>

class __declspec(dllexport) EMMediaDebugLog
{
public:
	static EMMediaDebugLog* Instance();
	static void Delete();

	EMMediaDebugLog();
	~EMMediaDebugLog();
	void operator<<(const char* p_vpString);
	void Log(const char* p_vpString);

private:
	static EMMediaDebugLog* m_opInstance;
	HANDLE m_uSemaphore;
	ofstream* m_opLogFile;
};

#endif