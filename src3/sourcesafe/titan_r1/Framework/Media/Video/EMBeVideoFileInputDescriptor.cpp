/*******************************************************
* Creator: Jesper Svensson
* Portability: hon-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#include "EMBeVideoFileInputDescriptor.h"
#include "EMBeMediaUtility.h"
#include "EMMediaDataBuffer.h"

EMBeVideoFileInputDescriptor::EMBeVideoFileInputDescriptor(string p_oFileName) 
	:	EMBeFileInputDescriptor(p_oFileName, EM_TYPE_ANY_VIDEO),
		m_opVideoFile(NULL)
{
	EMBeMediaUtility::push(this, "EMBeVideoFileInputDescriptor");
}

EMBeVideoFileInputDescriptor::~EMBeVideoFileInputDescriptor()
{
	delete m_opVideoFile;
	EMBeMediaUtility::pop("EMBeVideoFileInputDescriptor");
}

EMMediaFormat* EMBeVideoFileInputDescriptor::GetFormat()
{
	return m_opVideoFile -> GetFormat();
}

bool EMBeVideoFileInputDescriptor::InitCheckE()
{
	if(GetType() == EM_TYPE_RAW_VIDEO || GetType() == EM_TYPE_ANY_VIDEO || GetType() == EM_TYPE_ENCODED_VIDEO)
	{
		if(m_opVideoFile == NULL)
			m_opVideoFile = EMVideoFile::Create(GetFileName());
	}
	else
		EMDebugger("ERROR! Only raw audio implemented so far, in EMBeFileInputDescriptor::InitCheckE");
	
	if(! m_opVideoFile -> InitCheckE())
	{
		delete m_opVideoFile;
		return false;
	}

	//Calculate the duration of the media file, and store it in our member-attribute
//	EMMediaFormat* opFormat = m_opVideoFile -> ReadFormatE();
//	int64 vNumVideoFrames = m_opVideoFile -> NumberOfFramesInFile();
//	int64 vVideoTime = EMBeMediaUtility::FramesToTime(vNumVideoFrames, opFormat);

	m_vNumFrames = EMBeMediaUtility::TimeToFrames(m_opVideoFile -> GetDuration(), EMBeMediaUtility::GetSystemAudioFormat());
	return true;
}

bool EMBeVideoFileInputDescriptor::ReadFrames(EMMediaDataBuffer** p_opPrimaryBuffer, EMMediaDataBuffer** p_opSecondaryBuffer, int64 p_vMediaFramePosition, int64 p_vOffsetFrames, int64 & p_vOutNumRead, bool p_vSeeking)
{
	//Convert the offset value from "frames" into "bytes"
	int64 vOffset = 0;// EMBeMediaUtility::FramesToBytes(p_vOffsetFrames, m_opVideoFile -> GetFormat());
	int64 vMediaTimePosition = EMBeMediaUtility::FramesToTime(p_vMediaFramePosition, EMBeMediaUtility::GetSystemAudioFormat());// m_opVideoFile -> GetFormat());
	//Fill the buffer by sending it into the file's read-method, and also specify the buffer-offset in "bytes"
	p_vOutNumRead = m_opVideoFile -> ReadData(p_opPrimaryBuffer , p_opSecondaryBuffer, vMediaTimePosition, vOffset, p_vSeeking);
	if(p_vOutNumRead == 0)
	{
		//EMDebugger("SORRY NO FRAMES READ!");
		return false;
	}

	//If the read-operation read less than what was available in the buffer (offset accounted for)
	//we've most likely	reached the end-of-file, so we'll fill the rest of the buffer up with zeroes.
/*
	if(p_vOutNumRead < (p_opBuffer -> m_vSizeAvailable - vOffset))
	{
		char* vpEndPointPointer = static_cast<char*>(p_opBuffer -> Data());
		vpEndPointPointer += (vOffset + p_vOutNumRead);
		memset(vpEndPointPointer, 0, (p_opBuffer -> m_vSizeAvailable - vOffset) - p_vOutNumRead);
//		p_opBuffer -> m_vSizeUsed = p_opBuffer -> m_vSizeAvailable;
		p_vOutNumRead = p_opBuffer -> m_vSizeAvailable;
	}
*/
	return true;		
}

bool EMBeVideoFileInputDescriptor::SeekTo(int64 p_vNewMediaFrame)
{
	m_opVideoFile -> SeekToFrame(p_vNewMediaFrame);
	return true;
}

bool EMBeVideoFileInputDescriptor::SeekAndDisplay(int64 p_vNewMediaFrame)
{
	//m_opVideoFile -> SeekToTime(p_vNewMediaFrame);
	EMDebugger("EMBeVideoFileInputDescriptor::SeekAndDisplay(int64 p_vNewMediaFrame)");
	return true;
}

#endif
