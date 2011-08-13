#include "EMVideoEffectFilter.h"

#include <memory.h>


EMVideoEffectFilter::EMVideoEffectFilter()
{
}

EMVideoEffectFilter::~EMVideoEffectFilter()
{
}


unsigned char EMVideoEffectFilter::Intensity(unsigned char *p_pData, int p_vRange)
{
	unsigned char *pData = p_pData;
	unsigned int vMax = 0;

	for (int i=0; i<p_vRange; ++i, ++pData) 
	{
		if (vMax < *pData)
		{
			vMax = *pData;
		}
	}

	return vMax;
}

bool EMVideoEffectFilter::ShowDialog()
{
	return false;
}
