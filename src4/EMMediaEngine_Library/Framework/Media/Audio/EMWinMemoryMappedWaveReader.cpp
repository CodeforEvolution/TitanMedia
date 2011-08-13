#include "EMWinMemoryMappedWaveReader.h"
#include "EMWaveFileReader.h"
#include "EMMediaFormat.h"

EMWinMemoryMappedWaveReader::EMWinMemoryMappedWaveReader(string p_oFileName)
  :	m_oFileName(p_oFileName),
	m_vIsInitialized(false),
	m_opFormat(NULL),
	m_hFile(INVALID_HANDLE_VALUE),
	m_hMapping(INVALID_HANDLE_VALUE),
	m_vOffset(-1),
	m_vSize(-1),
	m_opPointer(NULL)
{
}

EMWinMemoryMappedWaveReader::~EMWinMemoryMappedWaveReader()
{
	if(m_opFormat != NULL)
		delete m_opFormat;

	if(m_opPointer != NULL)
		UnmapViewOfFile(m_opPointer);

	if(m_hMapping != INVALID_HANDLE_VALUE)
		CloseHandle(m_hMapping);

	if(m_hFile != INVALID_HANDLE_VALUE)
		CloseHandle(m_hFile);

}

bool EMWinMemoryMappedWaveReader::InitCheckE()
{
	if(m_vIsInitialized)
		return false;

	m_hFile = CreateFile(m_oFileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(INVALID_HANDLE_VALUE == m_hFile)
		return false;

	m_hMapping = CreateFileMapping(m_hFile, NULL, PAGE_READONLY, 0, 0, NULL);

	if(INVALID_HANDLE_VALUE == m_hMapping)
		return false;

	m_opPointer = static_cast<char*>(MapViewOfFile(m_hMapping, FILE_MAP_READ, 0, 0, 0));

	EMWaveHeader* opWave = (EMWaveHeader*) m_opPointer;

	if(_strnicmp(opWave -> RiffTag.id, "RIFF", 4) == 0)
	{
		if(_strnicmp(opWave -> WaveTag.format, "WAVE", 4) == 0)
		{
			if(_strnicmp(opWave -> FormatTag.id, "fmt ", 4) == 0)
			{
				m_opFormat = EM_new EMMediaFormat(EM_TYPE_RAW_AUDIO);

				m_opFormat -> m_vNumChannels = opWave -> Format.chans;
				m_opFormat -> m_vFrameRate = opWave -> Format.samplerate;
				m_opFormat -> m_vBytesPerSample = opWave -> Format.bitspersample / 8;

				char* opSearch = reinterpret_cast<char*>(&opWave -> Format) + opWave -> FormatTag.size;
				while(_strnicmp(opSearch, "data", 4) != 0)
				{
					opSearch += 8 + *reinterpret_cast<int32*>(opSearch + 4);
				}

				m_vOffset = opSearch + 8 - m_opPointer;
				m_vSize = *reinterpret_cast<int32*>(opSearch + 4);

				m_vIsInitialized = true;

				return true;
			}
		}
	}

	return false;
}

	
EMMediaFormat* EMWinMemoryMappedWaveReader::GetFormat()
{
	return m_opFormat;
}

int64 EMWinMemoryMappedWaveReader::NumberOfFramesInFile()
{
	return m_vSize / (m_opFormat -> m_vBytesPerSample * m_opFormat -> m_vNumChannels);
}

char* EMWinMemoryMappedWaveReader::GetDataPointer(int64 p_vStartFrame, int64 p_vStopFrame)
{
		return m_opPointer + m_vOffset + p_vStartFrame * m_opFormat -> m_vBytesPerSample * m_opFormat -> m_vNumChannels;
}

