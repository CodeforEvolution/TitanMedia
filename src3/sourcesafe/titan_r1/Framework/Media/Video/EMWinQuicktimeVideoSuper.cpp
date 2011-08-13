
#include "EMWinQuicktimeVideoSuper.h"
#include "EMWinQuicktimeVideo.h"
#include "EMMediaDataBuffer.h"

EMWinQuicktimeVideoSuper::EMWinQuicktimeVideoSuper(std::string p_oFileName)
{
	try
	{
		m_opQuicktimeVideo = new EMWinQuicktimeVideo(p_oFileName);
	}
	catch(...)
	{
		m_opQuicktimeVideo = NULL;
	}
}

EMWinQuicktimeVideoSuper::~EMWinQuicktimeVideoSuper()
{
	delete m_opQuicktimeVideo;
	m_opQuicktimeVideo = NULL;
}

bool EMWinQuicktimeVideoSuper::InitCheckE()
{
	return m_opQuicktimeVideo->InitCheckE();
}

_int32 EMWinQuicktimeVideoSuper::GetDuration()
{
	return m_opQuicktimeVideo -> GetDuration();
}

int EMWinQuicktimeVideoSuper::GetWidth()
{
	return m_opQuicktimeVideo -> GetWidth();
}
int EMWinQuicktimeVideoSuper::GetHeight()
{
	return m_opQuicktimeVideo -> GetHeight();
}

int EMWinQuicktimeVideoSuper::GetDepth()
{
	return m_opQuicktimeVideo -> GetDepth();
}

HBITMAP EMWinQuicktimeVideoSuper::GetDIBAtTime(int p_Width, int p_Height, _int64 p_vTime)
{
	return m_opQuicktimeVideo -> GetDIBAtTime(p_Width, p_Height, p_vTime);
}

bool EMWinQuicktimeVideoSuper::CopyFrameIntoBuffer(_int64 p_vTime, EMMediaDataBuffer* m_opMediaDataBuffer)
{
	return m_opQuicktimeVideo -> CopyFrameIntoBuffer(p_vTime, m_opMediaDataBuffer);
}

bool EMWinQuicktimeVideoSuper::CheckIfQuickTimeMovie(std::string* p_oFileName, bool p_vAudioVideo)
{
	return m_opQuicktimeVideo -> CheckIfQuickTimeMovie(p_oFileName, p_vAudioVideo);
}
