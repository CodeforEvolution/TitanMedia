/*******************************************************
* Creator: Martin Johansson
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/

#ifndef __EM_NEGATIVE_FILTER
#define __EM_NEGATIVE_FILTER

#include "EMVideoEffectFilter.h"

class EMNegativeFilter : public EMVideoEffectFilter
{
public:
	EMNegativeFilter();
	~EMNegativeFilter();

	string GetName();

	void Filter(void *p_vpSrcBitmap, void *p_vpDstBitmap);
	bool InitFilter(int p_vWidth, int p_vHeight, int p_BpP, int p_vPadBytes, float p_vNewNegative);

	bool NeedBackBuffer();

	EMVideoEffectFilter* Clone();

	bool LoadSetting(EMProjectDataLoader* p_opLoader);
	bool SaveSetting(EMProjectDataSaver* p_opSaver);

protected:
private:

};

#endif