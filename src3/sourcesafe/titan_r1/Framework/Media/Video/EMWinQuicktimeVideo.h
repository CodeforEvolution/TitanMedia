
#ifndef __EM_WIN_QUICKTIME_VIDEO
#define __EM_WIN_QUICKTIME_VIDEO

#include <windows.h>
#include "stdio.h"
#include "QTML.h"
#include "Movies.h"
#include "TextUtils.h"
#include "Quickdraw.h"
#include <string>
#include "EMScalingUtils.h"
#include "EMMediaDataBuffer.h"
//#include "EMVideoPreview.h"

class EMWinQuicktimeVideo
{
public:
	EMWinQuicktimeVideo(std::string p_oFileName);
	~EMWinQuicktimeVideo();
	bool InitCheckE();
	_int32 GetDuration();
	int GetWidth();
	int GetHeight();
	int GetDepth();
	HBITMAP GetDIBAtTime(int p_Width, int p_Height, _int64 p_vTime);
	bool CopyFrameIntoBuffer(_int64 p_vTime, EMMediaDataBuffer* m_opMediaDataBuffer);
	bool CheckIfQuickTimeMovie(std::string* p_oFileName, bool p_vAudioVideo);
private:
	void* DoCreateDIBSection(WORD	depth, LONG	width, LONG height);

private:

	EMMediaFormat* m_opSource;
	EMMediaFormat* m_opDestination;

	bool m_vIsInitialized;
	std::string m_oFileName;
	//Quicktime included 2001-10-01
	Movie m_oMovie;		// Movie identifier
	Rect m_sNaturalBounds;
	int m_vDepth;
	_int32 m_vDuration;
	TimeScale m_movieTimeScale;
	GWorldPtr m_gSrcGWorld;

	//for the windows DIB
	HDC m_hdcDest;
	HBITMAP m_dibHBitmap;
	void*	m_bits;
	EMScalingUtils	m_oScale;
	//EMVideoPreview* m_opVideoPreview;
	HBITMAP m_hbitmapSrc;
	HDC m_hdcSrc;
	FSSpec m_fileSpec;	// Descriptive information on file to open
	short m_theFile;		// Reference number of movie file

	CGrafPtr m_gOldPort;                        // Previous current port
	GDHandle m_gOldDevice;                      // Previous current device

};

#endif

