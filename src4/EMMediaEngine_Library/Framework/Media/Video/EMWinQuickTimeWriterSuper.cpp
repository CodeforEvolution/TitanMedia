#include "EMWinQuickTimeWriterSuper.h"

#include "EMWinQuickTimeWriter.h"

#include "EMMediaGlobals.h"

EMWinQuickTimeWriterSuper* EMWinQuickTimeWriterSuper::m_opQuickTimeWriterSuper = NULL;

EMWinQuickTimeWriterSuper::EMWinQuickTimeWriterSuper()
{
	m_opQuickTimeWriter = EMWinQuickTimeWriter::Instance();
}

EMWinQuickTimeWriterSuper::~EMWinQuickTimeWriterSuper()
{
}

EMWinQuickTimeWriterSuper* EMWinQuickTimeWriterSuper::Instance()
{
	if(m_opQuickTimeWriterSuper != NULL)
		return m_opQuickTimeWriterSuper;

	if(m_opQuickTimeWriterSuper == NULL)
		m_opQuickTimeWriterSuper = EM_new EMWinQuickTimeWriterSuper();

	return m_opQuickTimeWriterSuper;
}

void EMWinQuickTimeWriterSuper::SetRenderName(char* p_vpName)
{
	m_opQuickTimeWriter -> SetRenderName(p_vpName);
}

bool EMWinQuickTimeWriterSuper::InitCheckE(EMMediaType p_vMediaType)
{
	return m_opQuickTimeWriter -> InitCheckE(p_vMediaType);
}

void EMWinQuickTimeWriterSuper::GetAudioEncoderIDs(unsigned long* p_vpIDs)
{
	m_opQuickTimeWriter -> GetAudioEncoderIDs(p_vpIDs);
}
void EMWinQuickTimeWriterSuper::GetVideoEncoderIDs(unsigned long* p_vpIDs)
{
	m_opQuickTimeWriter -> GetVideoEncoderIDs(p_vpIDs);
}

int EMWinQuickTimeWriterSuper::GetNumberOfVideoEncoders()
{
	return m_opQuickTimeWriter -> GetNumberOfVideoEncoders();
}

int EMWinQuickTimeWriterSuper::GetNumberOfAudioEncoders()
{
	return m_opQuickTimeWriter -> GetNumberOfAudioEncoders();
}

void EMWinQuickTimeWriterSuper::SetShuffleFormatForVideo(EMMediaFormat* p_opMediaFormat)
{
	m_opQuickTimeWriter -> SetShuffleFormatForVideo(p_opMediaFormat);
}

void EMWinQuickTimeWriterSuper::SetShuffleFormatForAudio(EMMediaFormat* p_opMediaFormat)
{
	m_opQuickTimeWriter -> SetShuffleFormatForAudio(p_opMediaFormat);
}

void EMWinQuickTimeWriterSuper::SetAudioEncoderToUse(long p_vEncoderID)
{
	m_opQuickTimeWriter -> SetAudioEncoderToUse(p_vEncoderID);
}
void EMWinQuickTimeWriterSuper::SetVideoEncoderToUse(long p_vEncoderID)
{
	m_opQuickTimeWriter -> SetVideoEncoderToUse(p_vEncoderID);
}

void EMWinQuickTimeWriterSuper::DisConnectWithEncoders()
{
	m_opQuickTimeWriter -> DisConnectWithEncoders();
}

void EMWinQuickTimeWriterSuper::StopWritingToDisk()
{
	m_opQuickTimeWriter -> StopWritingToDisk();
}

void EMWinQuickTimeWriterSuper::ProcessVideoBuffer(EMMediaDataBuffer* p_opDataBuffer)
{
	m_opQuickTimeWriter -> ProcessVideoBuffer(p_opDataBuffer);
}

void EMWinQuickTimeWriterSuper::ProcessAudioBuffer(EMMediaDataBuffer* p_opDataBuffer)
{
	m_opQuickTimeWriter -> ProcessAudioBuffer(p_opDataBuffer);
}

void EMWinQuickTimeWriterSuper::SetCompressionCapabilityInfo(int32 p_vEncoderID, int32  p_vPropertyID)
{
	m_opQuickTimeWriter -> SetCompressionCapabilityInfo(p_vEncoderID, p_vPropertyID);
}

char* EMWinQuickTimeWriterSuper::GetCompressionCapabilities(int32 p_vEncoderID)
{
	return m_opQuickTimeWriter -> GetCompressionCapabilities(p_vEncoderID);
}

std::string EMWinQuickTimeWriterSuper::GetEncoderName(int32 p_vEncoderID)
{
	return m_opQuickTimeWriter -> GetEncoderName(p_vEncoderID);
}

bool EMWinQuickTimeWriterSuper::GetEncoderPropertyPages(int32 p_vEncoderID, bool p_vShouldShow)
{
	return m_opQuickTimeWriter -> GetEncoderPropertyPages(p_vEncoderID, p_vShouldShow);
}

bool EMWinQuickTimeWriterSuper::ExtractAudioFromQuickTimeFile(const char* p_opSource, const char* p_opDestination)
{
	return m_opQuickTimeWriter -> ExtractAudioFromQuickTimeFile(p_opSource, p_opDestination);
}