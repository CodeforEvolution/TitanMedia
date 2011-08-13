/*******************************************************
* Creator: Martin Johansson
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/

#ifndef __EM_BRIGHTNESS_FILTER
#define __EM_BRIGHTNESS_FILTER

#include "EMVideoEffectFilter.h"

#include <windows.h>

class EMBrightnessFilter : public EMVideoEffectFilter
{
public:
	EMBrightnessFilter();
	~EMBrightnessFilter();
	
	string GetName();

	void Filter(void *p_vpSrcBitmap, void *p_vpDstBitmap);
	bool InitFilter(int p_vWidth, int p_vHeight, int p_vBpP, int p_vPadBytes, float p_vNewContrast);

	bool NeedBackBuffer();

	EMVideoEffectFilter* Clone();

	bool LoadSetting(EMProjectDataLoader* p_opLoader);
	bool SaveSetting(EMProjectDataSaver* p_opSaver);

protected:
	unsigned char m_vLookupTable[256];
	int	  m_vConstant;
private:
};

#endif