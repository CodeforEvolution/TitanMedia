/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_FILE_MANAGER
#define __EM_FILE_MANAGER

#include <list>
#include <string>

class EMFileManager
{
public:
	virtual bool FileExists(const char* p_vpFileName) = 0;
	static EMFileManager* Instance();
	virtual list<string>* ListFiles(const char* p_vpDirectory) = 0;
	virtual bool MoveFileOrDirectory(const char* p_vpCurrentFileName, const char* p_vpNewFileName) = 0;

protected:
	list<string> m_oCurrentList;

private:
	static EMFileManager* m_opInstance;
};

#endif