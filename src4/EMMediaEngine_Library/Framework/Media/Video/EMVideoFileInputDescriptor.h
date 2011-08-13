/*******************************************************
* Creator: Jesper Svensson
* Portability: Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_VIDEO_FILE_INPUT_DESCRIPTOR
#define __EM_VIDEO_FILE_INPUT_DESCRIPTOR

#include "EMFileInputDescriptor.h"
#include "EMVideoFile.h"

class EMMediaFormat;

class EMVideoFileInputDescriptor : public EMFileInputDescriptor
{
public:
	EMVideoFileInputDescriptor(string p_oFileName);
	~EMVideoFileInputDescriptor();
	EMMediaFormat* GetFormat();
	bool InitCheckE();
	bool ReadFrames(EMMediaDataBuffer** p_opPrimaryBuffer, EMMediaDataBuffer** p_opSecondaryBuffer, int64 p_vMediaFramePosition, int64 p_vOffsetFrames, int64 & p_vOutNumRead, bool p_vSeeking = false);
	bool SeekTo(int64 p_vNewMediaFrame);
	bool SeekAndDisplay(int64 p_vNewMediaFrame);
	bool Open();
	EMVideoFile* m_opVideoFile;
private:
};

#endif

#endif
