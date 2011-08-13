#include "EMHSVFilter.h"

#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"

#include "EMGlobals.h"

#include "EMCommandRepository.h"
#include "CommandIDs.h"

EMHSVFilter::EMHSVFilter()
{
	m_vHSV[0] = 0;
	m_vHSV[1] = 0;
	m_vHSV[2] = 0;

	m_vInvert[0] = 1;
	for(int i = 1; i < 256; ++i)
		m_vInvert[i] = 1.0f / static_cast<float>(i);

	SetExtendedSettings(m_vHSV);
}

EMHSVFilter::~EMHSVFilter()
{
}


void EMHSVFilter::rgb2hsv(float *p_vpHSV, unsigned char* p_vpRGB)
//	0 < h < 6.0, 0 < s < 1.0, 0 < v < 256.0 
{
	unsigned char* vpB = p_vpRGB;
	unsigned char* vpG = p_vpRGB + 1;
	unsigned char* vpR = p_vpRGB + 2;

	int v = __max(__max(*vpB, *vpG), *vpR);
	int d = v - __min(__min(*vpB, *vpG), *vpR);
	float h;

	float s = v == 0 ? 0 : static_cast<float>(d) * m_vInvert[v];

	if(d > 0)
	{
		if(v == *vpR)
		{
			h = static_cast<float>(*vpG - *vpB) * m_vInvert[d];
		}
		else if(v == *vpG)
		{
			h = 2.0 + static_cast<float>(*vpB - *vpR) * m_vInvert[d];
		}
		else
		{
			h = 4.0 + static_cast<float>(*vpR - *vpG) * m_vInvert[d];
		}		

		if(h < 0)
			h += 6.0f;
	}
	else
	{
		h = 0;
	}

	p_vpHSV[0] = h;
	p_vpHSV[1] = s;
	p_vpHSV[2] = v;

}


void EMHSVFilter::hsv2rgb(float* p_vpHSV, unsigned char* p_vpRGB)
{
	float r;
	float g;
	float b;

	float h = static_cast<float>(*p_vpHSV);
	float v = static_cast<float>(*(p_vpHSV + 2));
	float s = v * static_cast<float>(*(p_vpHSV + 1));

	float k = static_cast<int>(h);
	
	if(k>h)
		--k;

	switch(static_cast<int>(k))
	{
	case 0:
		r = v;
		g = v - s * (1.0 - (h - k));
		b = v - s;
		break;
	case 1:
		r = v - s * (h - k);
		g = v;
		b = v - s;
		break;
	case 2:
		r = v - s;
		g = v;
		b = v - s * (1.0 - (h - k));
		break;
	case 3:
		r = v - s;
		g = v - s * (h - k);
		b = v;
		break;
	case 4:
		r = v - s * (1.0 - (h - k));
		g = v - s;
		b = v;
		break;
	case 5:
		r = v;
		g = v - s;
		b = v - s * (h - k);
		break;
	}

	*p_vpRGB = static_cast<unsigned char>(b + 0.5);
	*(p_vpRGB + 1) = static_cast<unsigned char>(g + 0.5);
	*(p_vpRGB + 2) = static_cast<unsigned char>(r + 0.5);
}



void EMHSVFilter::Filter(void *p_vpSrcBitmap, void *p_vpDstBitmap)
{
	unsigned char *pSrc = (unsigned char *)p_vpSrcBitmap;
	unsigned char *pDst = (unsigned char *)p_vpDstBitmap;
	float vHSV[3];

	int vMax = m_vWidth*m_vHeight;

	for (int y = 0; y < m_vHeight; ++y)
	{
		for (int x = 0; x < m_vWidth; ++x, pDst += 3, pSrc +=3)
		{
//			hsv_transform(pSrc, pDst);


			rgb2hsv(vHSV, pSrc);

			vHSV[0] += m_vHSV[0];

			if(vHSV[0] >= 6.0f)
				vHSV[0] -= 6.0f;

			if(vHSV[0] <0)
				vHSV[0] += 6.0f;


			vHSV[1] += m_vHSV[1];

			if(vHSV[1] >1)
				vHSV[1] = 1;

			if(vHSV[1] <0)
				vHSV[1] = 0;


			vHSV[2] += m_vHSV[2];

			if(vHSV[2] >255)
				vHSV[2] = 255;

			if(vHSV[2] <0)
				vHSV[2] = 0;

			hsv2rgb(vHSV, pDst);
		}

		pDst += m_vPadBytes;
		pSrc += m_vPadBytes;
	}
}	

bool EMHSVFilter::InitFilter(int p_vWidth, int p_vHeight, int p_vBpP, int p_vPadBytes, float p_vNewParameter)
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

bool EMHSVFilter::SetExtendedSettings(float* p_vpParams)
{
	m_vHSV[0] = p_vpParams[0];
	m_vHSV[1] = p_vpParams[1];
	m_vHSV[2] = p_vpParams[2];

	return true;
}

const float* EMHSVFilter::GetExtendedSettings()
{
	return m_vHSV;
}

bool EMHSVFilter::NeedBackBuffer()
{
	return false;
}

EMVideoEffectFilter* EMHSVFilter::Clone()
{
	EMVideoEffectFilter* opFilter = EM_new EMHSVFilter();

	opFilter -> InitFilter(m_vWidth, m_vHeight, m_vBpP, m_vPadBytes, m_vNewParameter);
	opFilter -> SetExtendedSettings(m_vHSV);

	return opFilter;
}

bool EMHSVFilter::LoadSetting(EMProjectDataLoader* p_opLoader)
{
	string test = p_opLoader -> LoadString();

	if(test != "EMHSVFilter -")
	{
		return false;
	}

	m_vWidth = p_opLoader -> LoadInt();
	m_vHeight = p_opLoader -> LoadInt();
	m_vPadBytes = p_opLoader -> LoadInt();
	m_vBpP = p_opLoader -> LoadInt();
	m_vNewParameter = p_opLoader -> LoadFloat();

	m_vHSV[0] = p_opLoader -> LoadFloat();
	m_vHSV[1] = p_opLoader -> LoadFloat();
	m_vHSV[2] = p_opLoader -> LoadFloat();

	test = p_opLoader -> LoadString();

	if(test != "- EMHSVFilter")
	{
		return false;
	}
	
	InitFilter(m_vWidth, m_vHeight, m_vBpP, m_vPadBytes, m_vNewParameter);
	SetExtendedSettings(m_vHSV);

	return true;
}

bool EMHSVFilter::SaveSetting(EMProjectDataSaver* p_opSaver)
{
	p_opSaver -> SaveString("EMHSVFilter -");

	p_opSaver -> SaveInt(m_vWidth);
	p_opSaver -> SaveInt(m_vHeight);
	p_opSaver -> SaveInt(m_vBpP);
	p_opSaver -> SaveInt(m_vPadBytes);
	p_opSaver -> SaveFloat(m_vNewParameter);

	p_opSaver -> SaveFloat(m_vHSV[0]);
	p_opSaver -> SaveFloat(m_vHSV[1]);
	p_opSaver -> SaveFloat(m_vHSV[2]);

	p_opSaver -> SaveString("- EMHSVFilter");

	return true;
}

string EMHSVFilter::GetName()
{
	return "EM HSV";
}

bool EMHSVFilter::ShowDialog()
{
		EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DISPLAY_HSV_VALUE_DIALOG, this);

	return true;
}
void EMHSVFilter::GetValues(float& p_vH, float& p_vS, float& p_vV)
{
	p_vH = 100.0f * m_vHSV[0] / 3.0f;
	p_vS = 100.0f * m_vHSV[1];
	p_vV = 100.0f * m_vHSV[2] / 255.0f;
}

void EMHSVFilter::SetValues(float p_vH, float p_vS, float p_vV)
{
	m_vHSV[0] = 3.0f * p_vH / 100.0f;
	m_vHSV[1] = p_vS / 100.0f;
	m_vHSV[2] = 255.0f * p_vV / 100.0f;
}