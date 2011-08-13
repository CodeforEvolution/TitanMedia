#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#include "EMBeAudioFileInputDescriptor.h"
#include "EMBeMediaUtility.h"
#include "EMMediaDataBuffer.h"
#include "EMMediaTimer.h"
#include "EMMediaFormat.h"

EMBeAudioFileInputDescriptor::EMBeAudioFileInputDescriptor(string p_oFileName) 
	:	EMBeFileInputDescriptor(p_oFileName, EM_TYPE_ANY_AUDIO),
		m_opWaveFile(NULL),
		m_opSystemAudioFormat(NULL)
{
	EMBeMediaUtility::push(this, "EMBeAudioFileInputDescriptor");
}

EMBeAudioFileInputDescriptor::~EMBeAudioFileInputDescriptor()
{
	delete m_opWaveFile;
	delete m_opSystemAudioFormat;
	EMBeMediaUtility::pop("EMBeAudioFileInputDescriptor");
}

EMMediaFormat* EMBeAudioFileInputDescriptor::GetFormat()
{
	return m_opWaveFile -> GetFormat();
}

bool EMBeAudioFileInputDescriptor::InitCheckE()
{
	if((GetType() & EM_TYPE_ANY_AUDIO) > 0)
		m_opWaveFile = EM_new EMWaveFileReader(GetFileName());
	else
		EMDebugger("ERROR! Only raw audio implemented so far, in EMBeFileInputDescriptor::InitCheckE");
	
	if(! m_opWaveFile -> InitCheckE())
	{
		delete m_opWaveFile;
		return false;
	}

	//Calculate the duration of the media file, and store it in our member-attribute
//	m_opWaveFile -> ReadFormatE();
	m_vNumFrames = m_opWaveFile -> NumberOfFramesInFile(); //EMBeMediaUtility::BytesToFrames(m_opWaveFile -> NumberOfFramesInFile(), sFormat);

	m_opSystemAudioFormat = EM_new EMMediaFormat(EM_TYPE_ANY_AUDIO);

	return true;
}

bool EMBeAudioFileInputDescriptor::ReadFrames(EMMediaDataBuffer** p_opPrimaryBuffer, EMMediaDataBuffer**, int64 p_vMediaFramePosition, int64 p_vOffsetFrames, int64 & p_vOutNumReadFrames, bool)
{
	//Convert the offset value from "frames" into "bytes"

	int64 vSilentBufferOffsetBytes = EMBeMediaUtility::FramesToBytes(p_vOffsetFrames, m_opWaveFile -> GetFormat());
	int64 vFromPositionBytes = EMBeMediaUtility::FramesToBytes(p_vMediaFramePosition, m_opWaveFile -> GetFormat());
	int64 vLimitBytes = EMBeMediaUtility::FramesToBytes(p_vOutNumReadFrames, m_opWaveFile -> GetFormat());
	//Fill the buffer by sending it into the file's read-method, and also specify the buffer-offset in "bytes"

	int64 vIOReadNum = ((*p_opPrimaryBuffer) -> m_vSizeAvailable > vLimitBytes ? vLimitBytes : (*p_opPrimaryBuffer) -> m_vSizeAvailable);
	m_opWaveFile -> ReadData((*p_opPrimaryBuffer) -> Data(), vIOReadNum, vFromPositionBytes, vSilentBufferOffsetBytes, EM_AUDIO_SAMPLESIZE);

	if(vIOReadNum == 0)
		return false;
		
	(*p_opPrimaryBuffer) -> m_oFormat.m_vNumChannels = EM_AUDIO_NUM_CHANNELS;
	(*p_opPrimaryBuffer) -> SetFrame(EMMediaTimer::Instance() -> AudioThenFrame());

	//If the read-operation read less than what was available in the buffer (offset accounted for)
	//we've most likely	reached the end-of-file, so we'll fill the rest of the buffer up with zeroes.
	if(vIOReadNum < ((*p_opPrimaryBuffer) -> m_vSizeAvailable - vSilentBufferOffsetBytes))
	{
		char* vpEndPointPointer = static_cast<char*>((*p_opPrimaryBuffer) -> Data());
		vpEndPointPointer += (vSilentBufferOffsetBytes + vIOReadNum);
		memset(vpEndPointPointer, 0, ((*p_opPrimaryBuffer) -> m_vSizeAvailable - vSilentBufferOffsetBytes) - vIOReadNum);
//		p_opBuffer -> m_vSizeUsed = p_opBuffer -> m_vSizeAvailable;
		//p_vOutNumRead = p_opBuffer -> m_vSizeAvailable;
	}
	p_vOutNumReadFrames = EMBeMediaUtility::BytesToFrames(vIOReadNum, m_opSystemAudioFormat);
	return true;		
}

bool EMBeAudioFileInputDescriptor::SeekTo(int64 p_vNewMediaFrame)
{
	m_opWaveFile -> SeekToFrame(p_vNewMediaFrame);
	return true;
}

bool EMBeAudioFileInputDescriptor::SeekAndDisplay(int64 p_vNewMediaFrame)
{
	return true;
}

#endif
