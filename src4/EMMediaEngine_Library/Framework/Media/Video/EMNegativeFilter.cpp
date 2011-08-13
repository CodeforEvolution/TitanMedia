#include "EMNegativeFilter.h"

#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"

#include "EMGlobals.h"


EMNegativeFilter::EMNegativeFilter()
{
}

EMNegativeFilter::~EMNegativeFilter()
{
}

void EMNegativeFilter::Filter(void *p_vpSrcBitmap, void *p_vpDstBitmap)
{
	unsigned char *pSrc = (unsigned char *)p_vpSrcBitmap;
	unsigned char *pDst = (unsigned char *)p_vpDstBitmap;

	int vMax = m_vWidth*m_vHeight;

	for (int y=0; y < m_vHeight; ++y)
	{
		for (int x=0; x < m_vWidth*m_vBpP; ++x)
		{
			*pDst = 255 - *pSrc;	
			++pDst;
			++pSrc;
		}
		pDst += m_vPadBytes;
		pSrc += m_vPadBytes;
	}
}	

bool EMNegativeFilter::InitFilter(int p_vWidth, int p_vHeight, int p_vBpP, int p_vPadBytes, float p_vNewParameter)
{
	m_vWidth		= p_vWidth;
	m_vHeight		= p_vHeight;
	m_vPadBytes		= p_vPadBytes;
	m_vBpP			= p_vBpP;
	m_vNewParameter = p_vNewParameter;

	return true;
}

bool EMNegativeFilter::NeedBackBuffer()
{
	return false;
}

EMVideoEffectFilter* EMNegativeFilter::Clone()
{
	EMVideoEffectFilter* opFilter = EM_new EMNegativeFilter();

	opFilter -> InitFilter(m_vWidth, m_vHeight, m_vBpP, m_vPadBytes, m_vNewParameter);

	return opFilter;
}

bool EMNegativeFilter::LoadSetting(EMProjectDataLoader* p_opLoader)
{
	string test = p_opLoader -> LoadString();

	if(test != "EMNegativeFilter -")
	{
		return false;
	}

	m_vWidth = p_opLoader -> LoadInt();
	m_vHeight = p_opLoader -> LoadInt();
	m_vPadBytes = p_opLoader -> LoadInt();
	m_vBpP = p_opLoader -> LoadInt();
	m_vNewParameter = p_opLoader -> LoadFloat();

	test = p_opLoader -> LoadString();

	if(test != "- EMNegativeFilter")
	{
		return false;
	}
	
	InitFilter(m_vWidth, m_vHeight, m_vBpP, m_vPadBytes, m_vNewParameter);

	return true;
}

bool EMNegativeFilter::SaveSetting(EMProjectDataSaver* p_opSaver)
{
	p_opSaver -> SaveString("EMNegativeFilter -");

	p_opSaver -> SaveInt(m_vWidth);
	p_opSaver -> SaveInt(m_vHeight);
	p_opSaver -> SaveInt(m_vBpP);
	p_opSaver -> SaveInt(m_vPadBytes);
	p_opSaver -> SaveFloat(m_vNewParameter);

	p_opSaver -> SaveString("- EMNegativeFilter");

	return true;
}

string EMNegativeFilter::GetName()
{
	return "EM Negative";
}

