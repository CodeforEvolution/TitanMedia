#include "EMWaveFileWriter.h"
#include "EMMediaDataBuffer.h"
#include "EMMediaIDManager.h"
#include "EMMediaFormat.h"
#include "EMBeMediaUtility.h"
#include "EMMediaFormat.h"
#include "EMMediaFile.h"
#include "EMMediaFileRepository.h"
#include "EMSemaphore.h"

#include <iostream>
#include <fstream>

EMWaveFileWriter::EMWaveFileWriter(string p_oFileName)
	:	m_oFileName(p_oFileName),
		m_opFile(NULL),
		m_vIsInitialized(false),
		m_opSem(NULL)
{
	m_vID = EMMediaIDManager::MakeID();
	m_opSem = EMSemaphore::CreateEMSemaphore("EMWaveFileWriterSemaphore");
}

EMWaveFileWriter::~EMWaveFileWriter()
{
	m_opSem -> Acquire();

	m_opFile -> Close();
	delete m_opFile;
	m_vIsInitialized = false;

	m_opSem -> Release();
}

int32 EMWaveFileWriter::GetID() const
{
	return m_vID;
}

bool EMWaveFileWriter::InitCheckE()
{
	m_opSem -> Acquire();

	m_opFile = EMMediaFile::Create(&m_oFileName);
	if(m_opFile == NULL)
		EMDebugger("ERROR! Could not find media file in file repository");
	m_opFile -> Open(EM_WRITE);
	char vpPad[44];
	m_opFile -> Write(vpPad, 44); //Make a header that's empty (unset)
	;//cout_commented_out_4_release << "Ok! WaveWriter have written 44 bytes of header space now, so filepos is " << m_opFile -> GetPosition() << endl;
	m_vIsInitialized = true;
	
	m_opSem -> Release();
	return true;
}

void EMWaveFileWriter::WriteHeaderE(EMMediaFormat* p_opFormat)
{
	m_opSem -> Acquire();

	if(! m_vIsInitialized)
	{
		eo << "ERROR! Wave file not initialized - Can't write header!" << ef;
		//release_sem(m_vSem);
		m_opSem -> Release();
		return;
	}

	EMWaveChunk sWaveChunk;
	EMWaveFormat sFormatChunk;

	m_opFile -> SeekFromStart(0);

	;//cout_commented_out_4_release << "==================>>>>> Now that header should be written, we are at position " << m_opFile -> GetPosition() << endl;

	int32 vFileLen = m_opFile -> GetLength();
	memcpy(sWaveChunk.id, "RIFF", 4);
	sWaveChunk.size = m_opFile -> GetLength() - 8;
	m_opFile -> Write(&sWaveChunk, 8);
	;//cout_commented_out_4_release << "Writing length = " << vFileLen << endl;
	
	memcpy(&(sWaveChunk.id), "WAVE", 4);
	m_opFile -> Write(&sWaveChunk, 4);
	
	memcpy(&(sWaveChunk.id), "fmt ", 4);
	sWaveChunk.size = 16;
	m_opFile -> Write(&sWaveChunk, 8);
	
	sFormatChunk.format = static_cast<int16>(1); //static_cast<int16>(p_opFormat -> m_vNumChannels);
	sFormatChunk.chans = static_cast<int16>(p_opFormat -> m_vNumChannels);
	sFormatChunk.samplerate = static_cast<int32>(p_opFormat -> m_vFrameRate);
	sFormatChunk.bitspersample = static_cast<int16>(p_opFormat -> m_vBytesPerSample * 8);
	sFormatChunk.blockalign = static_cast<int16>(sFormatChunk.chans * sFormatChunk.bitspersample / 8);
	sFormatChunk.byterate = static_cast<int32>(static_cast<int32>(sFormatChunk.samplerate) * static_cast<int32>(sFormatChunk.blockalign));
	m_opFile -> Write(&sFormatChunk, sizeof(sFormatChunk));
	
	eo << "Writing data chunk.." << ef;

	memcpy(&(sWaveChunk.id), "data", 4);
	sWaveChunk.size = vFileLen - 44; //Yet to be determined!
	m_opFile -> Write(&sWaveChunk, 8);

	eo << "Ok.. Wrote 8 bytes... perhaps!" << ef;

	eo << "Now that header has been written, we are at position " << (int) m_opFile -> GetPosition() << ef;

	m_opSem -> Release();
}

void EMWaveFileWriter::WriteData(void* p_upData, int64 p_vNumBytes)
{
	m_opSem -> Acquire();

	if(! m_vIsInitialized)
	{
		eo << "ERROR! Wave file not initialized - Can't write data!" << ef;
		m_opSem -> Release();
		return;
	}

	m_opFile -> Write(static_cast<char*>(p_upData), p_vNumBytes);
	m_opSem -> Release();
}
