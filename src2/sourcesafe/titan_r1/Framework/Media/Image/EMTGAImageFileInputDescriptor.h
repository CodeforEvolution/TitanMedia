/*******************************************************
* Creator: Martin Johansson
* Portability: Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_TGA_IMAGE_FILE_INPUT_DESCRIPTOR
#define __EM_TGA_IMAGE_FILE_INPUT_DESCRIPTOR

#include "EMImageFileInputDescriptor.h"

class EMTGAImageFileInputDescriptor : public EMImageFileInputDescriptor
{
public:
	EMTGAImageFileInputDescriptor(string p_oFileName);
	EMTGAImageFileInputDescriptor(const EMTGAImageFileInputDescriptor& p_oOther);
	~EMTGAImageFileInputDescriptor();

	bool InitCheckE();

	EMImageFileInputDescriptor* Clone();


private:
	unsigned char* Extract(unsigned char* p_opBits, int p_vDepth, unsigned char* p_opCMap = NULL, int p_vCDepth = 0);
	unsigned char* Convert(unsigned char* p_opBits, int p_vDepth, unsigned char* p_opCMap = NULL, int p_vCDepth = 0);
	void ConvertPixel(unsigned char* p_opSrc, unsigned char* p_opDst, int p_vDepth);

};

#endif

#endif
