/*******************************************************
* Creator: Martin Johansson
* Portability: Native
*-------------------------------------------------------
* 
* This class opens a file, finds out which format it is
* and maps it into the programs memory area.
*
*******************************************************/

#ifndef __EM_WIN_MEMORY_MAPPED_WAVE_WRITER
#define __EM_WIN_MEMORY_MAPPED_WAVE_WRITER

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#include <windows.h>

class EMMediaFormat;

class EMWinMemoryMappedWaveWriter
{
public:
	EMWinMemoryMappedWaveWriter(string p_oFileName, EMMediaFormat* p_opFormat, int64 p_vSize);
	~EMWinMemoryMappedWaveWriter();

	bool InitCheckE();

	char* GetDataPointer(int64 p_vStartFrame, int64 p_vStopFrame);
protected:
private:
	string m_oFileName;

	bool m_vIsInitialized;
	bool m_vIsMapped;

	EMMediaFormat* m_opFormat;

	HANDLE m_hFile;
	HANDLE m_hMapping;
	char* m_opPointer;
	int64 m_vSize;
};

#endif
