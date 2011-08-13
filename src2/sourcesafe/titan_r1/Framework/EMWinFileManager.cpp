#include "EMWinFileManager.h"

#ifdef PLATFORM_WINDOWS

#include <string>
#include <windows.h>

bool EMWinFileManager::FileExists(const char* p_vpFileName)
{
	WIN32_FIND_DATA sFindFile;
	if(FindFirstFile(p_vpFileName, &sFindFile) == INVALID_HANDLE_VALUE)
		return false;
	return true;
}

list<string>* EMWinFileManager::ListFiles(const char* p_vpDirectory)
{
	WIN32_FIND_DATA sFindFile;
	string oSearchString(p_vpDirectory);
	oSearchString = oSearchString + EM_FILE_SEPARATOR;
	oSearchString = oSearchString + "*.*";
	HANDLE opSearchHandle = FindFirstFile(oSearchString.c_str(), &sFindFile);

	if(opSearchHandle == INVALID_HANDLE_VALUE)
		return NULL;

	m_oCurrentList.clear();

	if(strcmp(sFindFile.cFileName, ".") != 0 && strcmp(sFindFile.cFileName, "..") != 0)
		m_oCurrentList.push_back(sFindFile.cFileName);

	bool vResult;
	while(true)
	{
		vResult = FindNextFile(opSearchHandle, &sFindFile) != 0;
		if(!vResult)
			break;
		if(strcmp(sFindFile.cFileName, ".") != 0 && strcmp(sFindFile.cFileName, "..") != 0)
			m_oCurrentList.push_back(sFindFile.cFileName);
	}

	return &m_oCurrentList;
}

bool EMWinFileManager::MoveFileOrDirectory(const char* p_vpCurrentFileName, const char* p_vpNewFileName)
{
	return MoveFile(p_vpCurrentFileName, p_vpNewFileName) != 0;
}

#endif