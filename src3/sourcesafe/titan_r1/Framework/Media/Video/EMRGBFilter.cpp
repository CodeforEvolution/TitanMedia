#include "EMRGBFilter.h"

#include "EMGlobals.h"

#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"

#include "EMCommandRepository.h"
#include "CommandIDs.h"


EMRGBFilter::EMRGBFilter()
{
	m_vRGB[0] = 0;
	m_vRGB[1] = 0;
	m_vRGB[2] = 0;

	SetExtendedSettings(m_vRGB);
}

EMRGBFilter::~EMRGBFilter()
{
}

void EMRGBFilter::Filter(void *p_vpSrcBitmap, void *p_vpDstBitmap)
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
				*pDst = m_vLookupTable[j][*pSrc];
			}

		}
		pDst += m_vPadBytes;
		pSrc += m_vPadBytes;
	}
}	

bool EMRGBFilter::InitFilter(int p_vWidth, int p_vHeight, int p_vBpP, int p_vPadBytes, float p_vNewParameter)
{
	if (m_vWidth != p_vWidth || m_vHeight != p_vHeight || m_vBpP != p_vBpP || m_vPadBytes != p_vPadBytes || m_vNewParameter != p_vNewParameter)
	{
		m_vWidth = p_vWidth;
		m_vHeight = p_vHeight;
		m_vPadBytes = p_vPadBytes;
		m_vBpP = p_vBpP;
		m_vNewParameter = p_vNewParameter;

		return true;
	}

	return false;
}

bool EMRGBFilter::SetExtendedSettings(float* p_vpParams)
{
	int i;
	int vSx;
	int vSy;

	for(i = 0; i < 3; ++i)
	{
		m_vRGB[i] = p_vpParams[i];

		int vNc = static_cast<int> (m_vRGB[i]);

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
		for (int j = 0; j < 256; ++j, t += k) 
		{
			m_vLookupTable[i][j] = t;
		}
	}

	return true;
}

const float* EMRGBFilter::GetExtendedSettings()
{
	return m_vRGB;
}

bool EMRGBFilter::NeedBackBuffer()
{
	return false;
}

EMVideoEffectFilter* EMRGBFilter::Clone()
{
	EMVideoEffectFilter* opFilter = EM_new EMRGBFilter();

	opFilter -> InitFilter(m_vWidth, m_vHeight, m_vBpP, m_vPadBytes, m_vNewParameter);
	opFilter -> SetExtendedSettings(m_vRGB);

	return opFilter;
}

bool EMRGBFilter::LoadSetting(EMProjectDataLoader* p_opLoader)
{
	string test = p_opLoader -> LoadString();

	if(test != "EMRGBFilter -")
	{
		return false;
	}

	m_vWidth = p_opLoader -> LoadInt();
	m_vHeight = p_opLoader -> LoadInt();
	m_vPadBytes = p_opLoader -> LoadInt();
	m_vBpP = p_opLoader -> LoadInt();
	m_vNewParameter = p_opLoader -> LoadFloat();

	m_vRGB[0] = p_opLoader -> LoadFloat();
	m_vRGB[1] = p_opLoader -> LoadFloat();
	m_vRGB[2] = p_opLoader -> LoadFloat();

	test = p_opLoader -> LoadString();

	if(test != "- EMRGBFilter")
	{
		return false;
	}
	
	InitFilter(m_vWidth, m_vHeight, m_vBpP, m_vPadBytes, m_vNewParameter);
	SetExtendedSettings(m_vRGB);

	return true;
}

bool EMRGBFilter::SaveSetting(EMProjectDataSaver* p_opSaver)
{
	p_opSaver -> SaveString("EMRGBFilter -");

	p_opSaver -> SaveInt(m_vWidth);
	p_opSaver -> SaveInt(m_vHeight);
	p_opSaver -> SaveInt(m_vBpP);
	p_opSaver -> SaveInt(m_vPadBytes);
	p_opSaver -> SaveFloat(m_vNewParameter);

	p_opSaver -> SaveFloat(m_vRGB[0]);
	p_opSaver -> SaveFloat(m_vRGB[1]);
	p_opSaver -> SaveFloat(m_vRGB[2]);

	p_opSaver -> SaveString("- EMRGBFilter");

	return true;
}

string EMRGBFilter::GetName()
{
	return "EM RGB";
}

bool EMRGBFilter::ShowDialog()
{
	EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DISPLAY_COLOR_BALANCE_DIALOG, this);
	return true;
}

void EMRGBFilter::GetValues(float& p_vR, float& p_vG, float& p_vB)
{
	p_vR = m_vRGB[2];
	p_vG = m_vRGB[1];
	p_vB = m_vRGB[0];

//	eo << "EMRGBFilter::GetValues(" << p_vR << ", " << p_vG << ", " << p_vB << ");" << ef;
}

void EMRGBFilter::SetValues(float p_vR, float p_vG, float p_vB)
{
	m_vRGB[2] = p_vR;
	m_vRGB[1] = p_vG;
	m_vRGB[0] = p_vB;

	SetExtendedSettings(m_vRGB);

//	eo << "EMRGBFilter::SetValues(" << p_vR << ", " << p_vG << ", " << p_vB << ");" << ef;
}


