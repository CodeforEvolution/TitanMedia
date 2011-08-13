/*******************************************************
* Creator: Martin Johansson
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/

#ifndef __EM_HSV_FILTER
#define __EM_HSV_FILTER

#include "EMVideoEffectFilter.h"

class __declspec(dllexport) EMHSVFilter : public EMVideoEffectFilter
{
public:
	EMHSVFilter();
	~EMHSVFilter();

	string GetName();

	void Filter(void *p_vpSrcBitmap, void *p_vpDstBitmap);
	bool InitFilter(int p_vWidth, int p_vHeight, int p_BpP, int p_vPadBytes, float p_vNewParam);
	bool SetExtendedSettings(float* p_vpParams);
	const float* GetExtendedSettings();

	bool NeedBackBuffer();

	EMVideoEffectFilter* Clone();

	bool LoadSetting(EMProjectDataLoader* p_opLoader);
	bool SaveSetting(EMProjectDataSaver* p_opSaver);

	bool ShowDialog();

	void GetValues(float& p_vH, float& p_vS, float& p_vV);
	void SetValues(float p_vH, float p_vS, float p_vV);

protected:
	float m_vHSV[3];
	float m_vInvert[256];

protected:
//	void hsv_transform(unsigned char* p_vpSrc, unsigned char* p_vpDst);

	void hsv2rgb(float* p_vpHSV, unsigned char* p_vpRGB);
	void rgb2hsv(float* p_vpHSV, unsigned char* p_vpRGB);


private:

};

#endif