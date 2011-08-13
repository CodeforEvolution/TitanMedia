/*******************************************************
* Creator: Richard Kronfalt
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#include <strmif.h>

#include <amvideo.h>
#include <qedit.h>
#include <stdio.h>

#ifndef __EM_MEDIA_FORMAT
#define __EM_MEDIA_FORMAT


class __declspec(dllexport) EMMediaFormat
{
public:
	void Copy(const EMMediaFormat* p_opFormat);
	void Display() const;
	EMMediaFormat(EMMediaType p_eType);
	EMMediaFormat(const EMMediaFormat& p_oFormat);
	~EMMediaFormat();
	EMMediaType m_eType;
	float m_vFrameRate;
	int32 m_vWidth;
	int32 m_vHeight;
	int32 m_vDepth;
	int64 m_vBufferSizeBytes;
	int32 m_vNumChannels;
	int32 m_vBytesPerSample;
	int64 m_vBytesPerRow;

	bool operator==(const EMMediaFormat& p_oOther);

#ifdef PLATFORM_BEOS
	void CreateFormat(const media_format* p_sFormat);
#endif

#ifdef PLATFORM_WINDOWS
	void CreateAudioFormat(const AM_MEDIA_TYPE* p_sFormat);
	void CreateVideoFormat(const AM_MEDIA_TYPE* p_sFormat);

//	void CreateFormat(const AM_MEDIA_TYPE* p_sFormat);
#endif
};

#endif
