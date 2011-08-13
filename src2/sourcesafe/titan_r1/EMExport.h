#ifndef __EM_EXPORT
#define __EM_EXPORT
#include "windows.h"

class __declspec(dllexport) EMExport
{
public:
	EMExport();
	~EMExport();
	bool Export(bool m_vVideo, bool m_vAudio, const char* vpFullPath);
	LPVOID Counter(bool executeORnot);
};

#endif