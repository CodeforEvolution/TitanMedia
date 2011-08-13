/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_FILE_MANAGER
#define __EM_WIN_FILE_MANAGER

#include "EMFileManager.h"

class EMWinFileManager : public EMFileManager
{
public:
	bool FileExists(const char* p_vpFileName);
	list<string>* ListFiles(const char* p_vpDirectory);
	bool MoveFileOrDirectory(const char* p_vpCurrentFileName, const char* p_vpNewFileName);
};

#endif

#endif