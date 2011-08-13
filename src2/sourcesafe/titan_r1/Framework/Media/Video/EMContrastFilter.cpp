#include "EMContrastFilter.h"

#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"

#include "EMGlobals.h"


EMContrastFilter::EMContrastFilter()
{
}

EMContrastFilter::~EMContrastFilter()
{
}

void EMContrastFilter::Filter(void *p_vpSrcBitmap, void *p_vpDstBitmap)
{
	unsigned char *pSrc = (unsigned char *)p_vpSrcBitmap;
	unsigned char *pDst = (unsigned char *)p_vpDstBitmap;

	int vMax = m_vWidth*m_vHeight;

	for (int y=0; y<m_vHeight; ++y)
	{
		for (int x=0; x<m_vWidth*m_vBpP; ++x)
		{
			*pDst = m_vLookupTable[*pSrc];	
			++pDst;
			++pSrc;
		}
		pDst += m_vPadBytes;
		pSrc += m_vPadBytes;
	}
}	

bool EMContrastFilter::InitFilter(int p_vWidth, int p_vHeight, int p_vBpP, int p_vPadBytes, float p_vNewParameter)
{
	if (m_vWidth != p_vWidth || m_vHeight != p_vHeight || m_vBpP != p_vBpP || m_vPadBytes != p_vPadBytes || m_vNewParameter != p_vNewParameter)
	{

		m_vWidth		= p_vWidth;
		m_vHeight		= p_vHeight;
		m_vPadBytes		= p_vPadBytes;
		m_vBpP			= p_vBpP;
		m_vNewParameter = p_vNewParameter;

		float vXs = 0;
		float vYs = 0;

		if (p_vNewParameter<0)
		{
			vYs = -p_vNewParameter;
			vXs = 0;
		} 
		else
		{
			vXs = p_vNewParameter;
			vYs = 0;
		}

		float k = (255 - 2*vYs) / (255 - 2*vXs);
		float m = vYs - k*vXs;
		float x = 0;

		for(int i = 0; i < 256; ++i, ++x)
		{
			float t = (k*x + m);

			t = t < 0 ? 0 : t;
			t = t > 255.0f ? 255.0f : t;

			m_vLookupTable[i] = t;
		}

		return true;
	}

	return false;
}

bool EMContrastFilter::NeedBackBuffer()
{
	return false;
}

EMVideoEffectFilter* EMContrastFilter::Clone()
{
	EMVideoEffectFilter* opFilter = EM_new EMContrastFilter();

	opFilter -> InitFilter(m_vWidth, m_vHeight, m_vBpP, m_vPadBytes, m_vNewParameter);

	return opFilter;
}

bool EMContrastFilter::LoadSetting(EMProjectDataLoader* p_opLoader)
{
	string test = p_opLoader -> LoadString();

	if(test != "EMContrastFilter -")
	{
		return false;
	}

	m_vWidth = p_opLoader -> LoadInt();
	m_vHeight = p_opLoader -> LoadInt();
	m_vPadBytes = p_opLoader -> LoadInt();
	m_vBpP = p_opLoader -> LoadInt();
	m_vNewParameter = p_opLoader -> LoadFloat();

	test = p_opLoader -> LoadString();

	if(test != "- EMContrastFilter")
	{
		return false;
	}
	
	InitFilter(m_vWidth, m_vHeight, m_vBpP, m_vPadBytes, m_vNewParameter);

	return true;
}

bool EMContrastFilter::SaveSetting(EMProjectDataSaver* p_opSaver)
{
	p_opSaver -> SaveString("EMContrastFilter -");

	p_opSaver -> SaveInt(m_vWidth);
	p_opSaver -> SaveInt(m_vHeight);
	p_opSaver -> SaveInt(m_vBpP);
	p_opSaver -> SaveInt(m_vPadBytes);
	p_opSaver -> SaveFloat(m_vNewParameter);

	p_opSaver -> SaveString("- EMContrastFilter");

	return true;
}

string EMContrastFilter::GetName()
{
	return "EM Contrast";
}

