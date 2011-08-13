/*******************************************************
* Creator: Martin Johansson
* Portability: Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_DX_IMAGE_FILE_INPUT_DESCRIPTOR
#define __EM_DX_IMAGE_FILE_INPUT_DESCRIPTOR


#include <dshow.h>
#include <streams.h>

#include "EMImageFileInputDescriptor.h"
#include "EMReadFrameInterface.h"
#include "EMWinQuicktimeVideoSuper.h"



class EMDXImageFileInputDescriptor : public EMImageFileInputDescriptor
{
public:
	EMDXImageFileInputDescriptor(string p_oFileName);
	EMDXImageFileInputDescriptor(const EMDXImageFileInputDescriptor& p_oOther);
	~EMDXImageFileInputDescriptor();

	bool InitCheckE();

	EMImageFileInputDescriptor* Clone();


private:
	IPin* FindOutputPin(IGraphBuilder* p_opIGraph);
	bool GetInternalFormat(IPin* p_opIPin);
	EMReadFrameInterface* ReplaceRenderWithGrab(IGraphBuilder* p_opIGraph, IPin* p_opIPin);

private:
	//Quicktime Approach
	EMWinQuicktimeVideoSuper* m_oQuickTimeVideo;

};

#endif

#endif
