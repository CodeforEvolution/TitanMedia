#include "EMWaveFileReader.h"
#include "EMMediaIDManager.h"
#include "EMMediaFormat.h"
#include "EMMediaUtility.h"
#include "EMMediaFormat.h"
#include "EMMediaFile.h"
#include "EMMediaFileRepository.h"
#include "EMSemaphore.h"
#include "EMGlobals.h"

EMWaveFileReader::EMWaveFileReader(string p_oFileName)
	:	m_oFileName(p_oFileName),
		m_opFile(NULL),
		m_vIsInitialized(false),
		m_vHasFormat(false),
		m_vSizeBytes(0),
		m_vHeaderLength(0),
		m_opFormat(NULL),
		m_opSem(NULL)
{
	m_vID = EMMediaIDManager::MakeID();
	m_opFormat = new EMMediaFormat(EM_TYPE_RAW_AUDIO);
	m_opSem = EMSemaphore::CreateEMSemaphore("EMWaveFileReader");
	m_vCurrentPosition = 0;
}

EMWaveFileReader::~EMWaveFileReader()
{
	m_opSem -> Acquire();

//	delete m_opFile;
	m_vIsInitialized = false;
	delete m_opFormat;
	
	m_opSem -> Release();
}

EMMediaFormat* EMWaveFileReader::GetFormat()
{
	if(! HasFormat())
		ReadFormatE();
	return m_opFormat;
}

int32 EMWaveFileReader::GetID() /*const*/
{
	return m_vID;
}

bool EMWaveFileReader::InitCheckE()
{
//	m_opSem -> Acquire();

	m_opFile = EMMediaFileRepository::Instance() -> FindFileName(&m_oFileName); //new ifstream();
	if(m_opFile == NULL)
	{
		EMMediaFileRepository::Instance() -> AddFile(&m_oFileName);
		m_opFile = EMMediaFileRepository::Instance() -> FindFileName(&m_oFileName); //new ifstream();
		if(m_opFile == NULL)
			EMDebugger("ERROR! Could still not find file in repository!");
	}

	m_opFile -> Open(EM_READ);
	
	m_vIsInitialized = ReadFormatE() == NULL ? false : true;

//	m_opSem -> Release();
	return m_vIsInitialized;
}

bool EMWaveFileReader::HasFormat() /*const*/
{
	return m_vHasFormat;
}

EMMediaFormat* EMWaveFileReader::ReadFormatE()
{
	if(m_vHasFormat && m_opFormat != NULL)
		return m_opFormat;

	EMWaveChunk opChunk;
	EMWaveRiff opRiffChunk;
	EMWaveFormat opFormatChunk;

	m_opFile -> SeekFromStart(0);
	m_vCurrentPosition = 0;
	m_vCurrentPosition += m_opFile -> Read(&opChunk, 8, m_vCurrentPosition);
	if(strncmp(opChunk.id, "RIFF", 4) == 0)
		m_vCurrentPosition += m_opFile -> Read(&opRiffChunk, 4, m_vCurrentPosition);	//Skip past 'WAVE'
	else
	{
		m_vHasFormat = false;
		return NULL;
	}

	m_vCurrentPosition += m_opFile -> Read(&opChunk, 8, m_vCurrentPosition);
	if(strncmp(opChunk.id, "fmt ", 4) == 0)
	{
		m_vCurrentPosition += m_opFile -> Read(&opFormatChunk, 16, m_vCurrentPosition);
		if(opChunk.size > 16)
			m_vCurrentPosition = m_vCurrentPosition + opChunk.size - 16;
	}

	m_vCurrentPosition += m_opFile -> Read(&opChunk, 8, m_vCurrentPosition);
	if(strncmp(opChunk.id, "PAD ", 4) == 0)
		m_vCurrentPosition = m_vCurrentPosition + opChunk.size;
	else if(strncmp(opChunk.id, "data", 4) == 0)
		m_vCurrentPosition = m_vCurrentPosition - 8;

	m_vCurrentPosition += m_opFile -> Read(&opChunk, 8, m_vCurrentPosition);
	if(strncmp(opChunk.id, "data ", 4) == 0)
	{
		m_opFormat -> m_eType = EM_TYPE_RAW_AUDIO;
		m_opFormat -> m_vFrameRate = opFormatChunk.samplerate;
		m_opFormat -> m_vNumChannels = opFormatChunk.chans;
		if(opFormatChunk.bitspersample == 8)
			m_opFormat -> m_vBytesPerSample = 1;
		else if(opFormatChunk.bitspersample == 16)
			m_opFormat -> m_vBytesPerSample = 2;
		else
			EMDebugger("Unknown audio sample format in file!"); //TODO: Throw exception instead?
		m_vSizeBytes = opChunk.size;
	}		
	else
	{
		//eo << "Audio file format is \"" << opChunk.id << "\"." << ef;
//		EMDebugger("Unknown audio file format!"); //TODO: Throw exception instead?
		m_vHasFormat = false;
		return NULL;
	}

	m_vHeaderLength = m_vCurrentPosition; //m_opFile -> GetPosition(); //m_opFile -> tellg();
	m_vHasFormat = true;
	return m_opFormat;
}

int64 EMWaveFileReader::NumberOfFramesInFile() /*const*/
{
	if(! HasFormat())
		ReadFormatE();

	return m_vSizeBytes / (m_opFormat -> m_vBytesPerSample * m_opFormat -> m_vNumChannels);
}

int64 EMWaveFileReader::CurrentBytePosition()
{
	if(! HasFormat())
		ReadFormatE();

	m_opSem -> Acquire();

	if(! m_vIsInitialized)
	{
		m_opSem -> Release();
		return 0;
	}
	
	int64 vCurrentPos = m_opFile -> GetPosition(); //(m_vIsReading ? m_opFile -> tellg() : m_opFile -> tellp());
//	if(m_opFile -> eof())
//		EMDebugger("ERROR! 22 File seems to be EOF!");
	m_opSem -> Release();
	return vCurrentPos;
}

void EMWaveFileReader::ReadData(void* p_vpBuffer, int64& p_vInOutNumBytes, int64 p_vBytePosition, int64 p_vBufferOffset, int32 p_vNumBytesPerSample)
{

	//eo << "Parameters: " << ef;
	//eo << "vpBuffer: " << (int) p_vpBuffer << ef;
	//eo << "p_vInOutNumBytes: " << (int) p_vInOutNumBytes << ef;
	//eo << "p_vBytePosition: " << (int) p_vBytePosition << ef;
	//eo << "p_vBufferOffset: " << (int) p_vBufferOffset << ef;

	if(! HasFormat())
		ReadFormatE();

	//m_opSem -> Acquire();

	if(m_vIsInitialized && m_vHasFormat)
	{
		p_vBytePosition += m_vHeaderLength; //Make sure we don't forget to include the header-length when addressing media file data!
		char* ptr = static_cast<char*>(p_vpBuffer);
		if(p_vBufferOffset > 0)
			memset(ptr, 0, p_vBufferOffset);

		ptr += p_vBufferOffset;
		
		int64 vGCount = m_opFile -> Read(ptr, p_vInOutNumBytes - p_vBufferOffset, p_vBytePosition);
		
		p_vInOutNumBytes = vGCount;
	}
	else
		EMDebugger("It's an error to try to Read a wave file when it's not opened and/or the format hasn't been read yet! Use InitCheckE and ReadFormatE first!"); //TODO: Throw exception instead?
	
	//m_opSem -> Release();
}

void EMWaveFileReader::SeekToFrame(int64 p_vToFrame)
{
	if(! HasFormat())
		ReadFormatE();

	m_opSem -> Acquire();

//	if(m_vIsInitialized)
//		m_opFile -> SeekFromStart(p_vToFrame * m_opFormat -> m_vNumChannels * m_opFormat -> m_vBytesPerSample); //, ios::beg);
	m_vCurrentPosition = p_vToFrame * m_opFormat -> m_vNumChannels * m_opFormat -> m_vBytesPerSample;
	
	m_opSem -> Release();
}

void EMWaveFileReader::SeekToTime(int64 p_vToTime)
{
	if(! HasFormat())
		ReadFormatE();

	m_opSem -> Acquire();

	if(m_vIsInitialized)
		SeekToFrame(EMMediaUtility::Instance() -> TimeToFrames(p_vToTime, m_opFormat));
	m_vCurrentPosition = EMMediaUtility::Instance() -> TimeToFrames(p_vToTime, m_opFormat);
	
	m_opSem -> Release();
}

int64 EMWaveFileReader::GetSize() /*const*/
{
	if(! HasFormat())
		ReadFormatE();
	return m_vSizeBytes;
}

string EMWaveFileReader::GetFileName() /*const*/
{
	return m_oFileName;
}
