#include "EMWinMemoryMappedWaveWriter.h"
#include "EMMediaFormat.h"
#include "EMWaveFileReader.h"


#include <windows.h>


EMWinMemoryMappedWaveWriter::EMWinMemoryMappedWaveWriter(string p_oFileName, EMMediaFormat* p_opFormat, int64 p_vFrames)
  :	m_oFileName(p_oFileName),
	m_opFormat(EM_new EMMediaFormat(*p_opFormat)),
	m_vIsInitialized(false),
	m_vIsMapped(false),
	m_hFile(INVALID_HANDLE_VALUE),
	m_hMapping(INVALID_HANDLE_VALUE),
	m_opPointer(NULL)
{
	m_vSize = sizeof(EMWaveHeader) + 8 + p_opFormat -> m_vNumChannels * p_opFormat -> m_vBytesPerSample * p_vFrames;
}

EMWinMemoryMappedWaveWriter::~EMWinMemoryMappedWaveWriter()
{
	if(m_opPointer != NULL)
		UnmapViewOfFile(m_opPointer);

	if(m_hMapping != INVALID_HANDLE_VALUE)
		CloseHandle(m_hMapping);

	if(m_hFile != INVALID_HANDLE_VALUE)
		CloseHandle(m_hFile);

	delete m_opFormat;
}

bool EMWinMemoryMappedWaveWriter::InitCheckE()
{
	m_hFile = CreateFile(m_oFileName.c_str(), GENERIC_WRITE | GENERIC_READ, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(m_hFile == INVALID_HANDLE_VALUE)
		return false;

	m_hMapping = CreateFileMapping(m_hFile, NULL, PAGE_READWRITE, (m_vSize & 0xffffffff00000000) >> 32, m_vSize & 0xffffffff, NULL);
	if(m_hMapping == INVALID_HANDLE_VALUE || m_hMapping == NULL)
	{
		DWORD errCode = GetLastError();
		return false;
	}

	m_opPointer = static_cast<char*>(MapViewOfFile(m_hMapping, FILE_MAP_ALL_ACCESS, 0, 0, m_vSize));

	if(m_opPointer == NULL)
	{
		DWORD errCode = GetLastError();
		return false;
	}
 

	EMWaveHeader* opWave = (EMWaveHeader*) m_opPointer;

	memcpy(&opWave -> RiffTag.id, "RIFF", 4);

	opWave -> RiffTag.size = m_vSize - 8;
	memcpy(&opWave -> WaveTag.format, "WAVE", 4);
	memcpy(&opWave -> FormatTag.id, "fmt ", 4);
	opWave -> FormatTag.size = sizeof(opWave -> Format);
	opWave -> Format.bitspersample = m_opFormat -> m_vBytesPerSample * 8;
	opWave -> Format.blockalign = m_opFormat -> m_vBytesPerSample * m_opFormat -> m_vNumChannels;	
	opWave -> Format.byterate = m_opFormat -> m_vFrameRate * m_opFormat -> m_vBytesPerSample * m_opFormat -> m_vNumChannels;
	opWave -> Format.chans = m_opFormat -> m_vNumChannels;	
	opWave -> Format.samplerate = m_opFormat -> m_vFrameRate;
	opWave -> Format.format = 1;

	memcpy(m_opPointer + sizeof(EMWaveHeader), "data", 4);
	*reinterpret_cast<int32*>(m_opPointer + sizeof(EMWaveHeader) + 4) = m_vSize - 8 - sizeof(EMWaveHeader);

	return true;
}

char* EMWinMemoryMappedWaveWriter::GetDataPointer(int64 p_vStartFrame, int64 p_vStopFrame)
{
	return m_opPointer + sizeof(EMWaveHeader) + 8 + p_vStartFrame * m_opFormat -> m_vBytesPerSample * m_opFormat -> m_vNumChannels;
}


