/*******************************************************
* Creator: Jesper Svensson
* Portability: Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_VIDEO_FILE_INPUT_DESCRIPTOR
#define __EM_BE_VIDEO_FILE_INPUT_DESCRIPTOR

#include "EMBeFileInputDescriptor.h"
#include "EMVideoFile.h"

class EMMediaFormat;

class EMBeVideoFileInputDescriptor : public EMBeFileInputDescriptor
{
public:
	EMBeVideoFileInputDescriptor(string p_oFileName);
	~EMBeVideoFileInputDescriptor();
	EMMediaFormat* GetFormat();
	bool InitCheckE();
	bool ReadFrames(EMMediaDataBuffer** p_opPrimaryBuffer, EMMediaDataBuffer** p_opSecondaryBuffer, int64 p_vMediaFramePosition, int64 p_vOffsetFrames, int64 & p_vOutNumRead, bool p_vSeeking = false);
	bool SeekTo(int64 p_vNewMediaFrame);
	bool SeekAndDisplay(int64 p_vNewMediaFrame);
	
private:
	EMVideoFile* m_opVideoFile;
};

#endif

#endif
