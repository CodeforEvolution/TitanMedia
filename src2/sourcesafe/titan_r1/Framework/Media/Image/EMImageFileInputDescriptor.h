/*******************************************************
* Creator: Martin Johansson
* Portability: Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_IMAGE_FILE_INPUT_DESCRIPTOR
#define __EM_IMAGE_FILE_INPUT_DESCRIPTOR

#include "EMFileInputDescriptor.h"
#include "EMMediaFormat.h"
#include "EMScalingUtils.h"



class EMImageFileInputDescriptor : public EMFileInputDescriptor
{
public:
	EMImageFileInputDescriptor(string p_oFileName);
	~EMImageFileInputDescriptor();

	EMMediaFormat* GetFormat();

//	virtual bool InitCheckE() = 0;
	virtual EMImageFileInputDescriptor* Clone() = 0;

	bool ReadFrames(EMMediaDataBuffer** p_opPrimaryBuffer, EMMediaDataBuffer** p_opSecondaryBuffer, int64 p_vMediaFramePosition, int64 p_vOffsetFrames, int64 & p_vOutNumRead, bool p_vSeeking = false);
	bool ReadFrames(EMMediaDataBuffer* p_opPrimaryBuffer);
	bool SeekTo(int64 p_vNewMediaFrame);
	bool SeekAndDisplay(int64 p_vNewMediaFrame);

protected:
	EMMediaFormat m_oFormat;
	EMMediaFormat m_oScaledFormat;
	unsigned char* m_vpOriginal;
	unsigned char* m_vpScaled;

	bool m_vSwapImage;

protected:
	EMScalingUtils m_oScale;
};

#endif

#endif
