/*******************************************************
* Creator: Martin Johansson
* Portability: Native
*-------------------------------------------------------
* 
* This class opens a file, finds out which format it is
* and maps it into the programs memory area.
*
*******************************************************/

#ifndef __EM_WIN_MEMORY_MAPPED_WAVE_READER
#define __EM_WIN_MEMORY_MAPPED_WAVE_READER

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#include <windows.h>

class EMMediaFormat;

class EMWinMemoryMappedWaveReader
{
public:
	EMWinMemoryMappedWaveReader(string p_oFileName);
	~EMWinMemoryMappedWaveReader();

	EMMediaFormat* GetFormat();

	bool InitCheckE();
	int64 NumberOfFramesInFile();

	char* GetDataPointer(int64 p_vStartFrame, int64 p_vStopFrame);

protected:
private:
	string m_oFileName;

	bool m_vIsInitialized;
	bool m_vIsMapped;

	EMMediaFormat* m_opFormat;

	int64 m_vOffset;
	int64 m_vSize;

	HANDLE m_hFile;
	HANDLE m_hMapping;
	char* m_opPointer;
};

#endif
