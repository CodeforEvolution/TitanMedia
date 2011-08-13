#include "EMBrightnessFilter.h"

#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"

#include "EMGlobals.h"

EMBrightnessFilter::EMBrightnessFilter()
{
}

EMBrightnessFilter::~EMBrightnessFilter()
{
}

void EMBrightnessFilter::Filter(void *p_vpSrcBitmap, void *p_vpDstBitmap)
{
	unsigned char *pSrc = (unsigned char *)p_vpSrcBitmap;
	unsigned char *pDst = (unsigned char *)p_vpDstBitmap;

	int vMax = m_vWidth*m_vHeight;

	for (int y=0; y<m_vHeight; ++y)
	{

		for (int x=0; x<m_vWidth; ++x)
		{
			for (int j=0; j<m_vBpP; ++j, ++pDst, ++pSrc) 
			{
				*pDst = m_vLookupTable[*pSrc];

			}

		}
		pDst += m_vPadBytes;
		pSrc += m_vPadBytes;
	}
}	

bool EMBrightnessFilter::InitFilter(int p_vWidth, int p_vHeight, int p_vBpP, int p_vPadBytes, float p_vNewParameter)
{
	if (m_vWidth != p_vWidth || m_vHeight != p_vHeight || m_vBpP != p_vBpP || m_vPadBytes != p_vPadBytes || m_vNewParameter != p_vNewParameter)
	{
		m_vWidth = p_vWidth;
		m_vHeight = p_vHeight;
		m_vPadBytes = p_vPadBytes;
		m_vBpP = p_vBpP;
		m_vNewParameter = p_vNewParameter;

		int vSx;
		int vSy;

		int vNc = static_cast<int> (255.0f * p_vNewParameter / 100.0f);

		if (vNc <0)
		{
			vSx = 0;
			vSy = 255 + vNc;
		}
		else
		{
			vSx = vNc;
			vSy = 255;
		}	

		float k = static_cast<float> (vSy - vSx) / 255.0f;
	
		float t = vSx;
		for (int i = 0; i < 256; ++i, t += k) 
		{
			m_vLookupTable[i] = t;
		}

		return true;
	}

	return false;
}


bool EMBrightnessFilter::NeedBackBuffer()
{
	return false;
}

EMVideoEffectFilter* EMBrightnessFilter::Clone()
{
	EMVideoEffectFilter* opFilter = EM_new EMBrightnessFilter();

	opFilter -> InitFilter(m_vWidth, m_vHeight, m_vBpP, m_vPadBytes, m_vNewParameter);

	return opFilter;
}

bool EMBrightnessFilter::LoadSetting(EMProjectDataLoader* p_opLoader)
{
	string test = p_opLoader -> LoadString();

	if(test != "EMBrightnessFilter -")
	{
		return false;
	}

	m_vWidth = p_opLoader -> LoadInt();
	m_vHeight = p_opLoader -> LoadInt();
	m_vPadBytes = p_opLoader -> LoadInt();
	m_vBpP = p_opLoader -> LoadInt();
	m_vNewParameter = p_opLoader -> LoadFloat();

	test = p_opLoader -> LoadString();

	if(test != "- EMBrightnessFilter")
	{
		return false;
	}
	
	InitFilter(m_vWidth, m_vHeight, m_vBpP, m_vPadBytes, m_vNewParameter);

	return true;
}

bool EMBrightnessFilter::SaveSetting(EMProjectDataSaver* p_opSaver)
{
	p_opSaver -> SaveString("EMBrightnessFilter -");

	p_opSaver -> SaveInt(m_vWidth);
	p_opSaver -> SaveInt(m_vHeight);
	p_opSaver -> SaveInt(m_vBpP);
	p_opSaver -> SaveInt(m_vPadBytes);
	p_opSaver -> SaveFloat(m_vNewParameter);

	p_opSaver -> SaveString("- EMBrightnessFilter");

	return true;
}

string EMBrightnessFilter::GetName()
{
	return "EM Brightness";
}
