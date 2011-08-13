/*******************************************************
* Creator: Martin Johansson
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/

#ifndef __EM_RGB_FILTER
#define __EM_RGB_FILTER

#include "EMVideoEffectFilter.h"

class __declspec(dllexport) EMRGBFilter : public EMVideoEffectFilter
{
public:
	EMRGBFilter();
	~EMRGBFilter();

	string GetName();

	void Filter(void *p_vpSrcBitmap, void *p_vpDstBitmap);
	bool InitFilter(int p_vWidth, int p_vHeight, int p_BpP, int p_vPadBytes, float p_vNewContrast);
	bool SetExtendedSettings(float* p_vpParams);
	const float* GetExtendedSettings();

	bool NeedBackBuffer();

	EMVideoEffectFilter* Clone();

	bool LoadSetting(EMProjectDataLoader* p_opLoader);
	bool SaveSetting(EMProjectDataSaver* p_opSaver);

	bool ShowDialog();

	void GetValues(float& p_vR, float& p_vG, float& p_vB);
	void SetValues(float p_vR, float p_vG, float p_vB);

protected:
	unsigned char m_vLookupTable[3][256];
	float m_vRGB[3];
private:

};

#endif