
#ifndef __EM_WIN_QUICKTIME_VIDEO_SUPER
#define __EM_WIN_QUICKTIME_VIDEO_SUPER

#include <windows.h>
#include <string>

class EMWinQuicktimeVideo;

class EMMediaDataBuffer;

class EMWinQuicktimeVideoSuper
{
public:
	EMWinQuicktimeVideoSuper(std::string p_oFileName);
	~EMWinQuicktimeVideoSuper();
	bool InitCheckE();
	_int32 GetDuration();
	int GetWidth();
	int GetHeight();
	int GetDepth();
	HBITMAP GetDIBAtTime(int p_Width, int p_Height, _int64 p_vTime);
	bool CopyFrameIntoBuffer(_int64 p_vTime, EMMediaDataBuffer* m_opMediaDataBuffer);
	bool CheckIfQuickTimeMovie(std::string* p_oFileName, bool p_vAudioVideo);

private:
	EMWinQuicktimeVideo* m_opQuicktimeVideo;
};

#endif