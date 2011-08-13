/*******************************************************
* Creator: Martin Johansson
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/

#ifndef __EM_VIDEO_EFFECT_FILTER
#define __EM_VIDEO_EFFECT_FILTER

class EMProjectDataLoader;
class EMProjectDataSaver;

#include "EMGlobals.h"

class __declspec(dllexport) EMVideoEffectFilter
{
public:
	EMVideoEffectFilter();
	~EMVideoEffectFilter();

	virtual string GetName() = 0;

	virtual void Filter(void *p_vpSrcBitmap, void *p_vpDstBitmap) = 0; 

	virtual bool InitFilter(int p_vWidth, int p_vHeight, int p_BpP, int p_vPadBytes, float p_vNewParameter) = 0;

	virtual bool NeedBackBuffer() = 0;

	virtual EMVideoEffectFilter* Clone() = 0;

	virtual bool LoadSetting(EMProjectDataLoader* p_opLoader) = 0;
	virtual bool SaveSetting(EMProjectDataSaver* p_opSaver) = 0;

	virtual bool SetExtendedSettings(float* p_vpParams) { return true;};
	virtual const float* GetExtendedSettings() { return NULL;};

	virtual bool ShowDialog();

protected:
	int m_vWidth;
	int m_vHeight;
	int m_vPadBytes;
	int m_vBpP;
	float m_vNewParameter;

protected:
	unsigned char Intensity(unsigned char *p_pData, int p_vRange);

private:

};

#endif
