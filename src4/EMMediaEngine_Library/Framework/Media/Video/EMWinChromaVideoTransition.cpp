#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMWinChromaVideoTransition.h"
#include "EMWinVideoShuffleOutputFilter.h"
#include "EMMediaSystemInspector.h"
#include "EMPlugin.h"

#include "EMMediaTransitionDialogIDs.h"

#include <streams.h>	


EMWinChromaVideoTransition::EMWinChromaVideoTransition(string p_oPluginName, IDirectDraw7* p_opIDD, IMoniker* p_opIMoniker)
	:	EMWinDirectXVideoTransition(p_oPluginName, p_opIDD, p_opIMoniker),
	m_opIDxt(NULL)
{
	GetDefaultSettings(reinterpret_cast<int32*>(&m_oParams));
	m_vSwap = true;
}

EMWinChromaVideoTransition::~EMWinChromaVideoTransition()
{
	if (m_opIDxt != NULL)
	{
		m_opIDxt -> Release();
		m_opIDxt = NULL;
	}
}

void EMWinChromaVideoTransition::GetDefaultSettings(int32* p_opParam)
{
	memset(p_opParam, 0, sizeof(EMDialogParameters));
	reinterpret_cast<EMDialogParameters*>(p_opParam) -> m_vID = EM_TRANSITION_KEY;
	reinterpret_cast<EMDialogParameters*>(p_opParam) -> m_vParams.m_vKey.m_vColorID = EM_KEY_BLUE;
	reinterpret_cast<EMDialogParameters*>(p_opParam) -> m_vParams.m_vKey.m_vTolerance = 100;
	reinterpret_cast<EMDialogParameters*>(p_opParam) -> m_vSwap = false;
}

bool EMWinChromaVideoTransition::UpdateSettings()
{
	HRESULT hr;

	EMDialogParameters oParam;

	if(m_opIDxt != NULL)
	{
		GetSettings(reinterpret_cast<int32*>(&oParam));
		m_opIDxt -> Release();
		m_opIDxt = NULL;
	}
	else
		GetDefaultSettings(reinterpret_cast<int32*>(&oParam));

	hr = m_opIEffect -> QueryInterface(IID_IDxtKey, (void**)&m_opIDxt);
	if(hr != S_OK)
		return false;

	SetSettings(reinterpret_cast<int32*>(&oParam));

	return true;
}

bool EMWinChromaVideoTransition::NeedReset()
{
	return false;
}

float EMWinChromaVideoTransition::CalcTime(EMMediaDataBuffer* p_opBuffer)
{
	return 1.0f; // - static_cast<float>(m_oParams.m_vParams.m_vKey.m_vTolerance) / 100;
}

bool EMWinChromaVideoTransition::CopyTransParams(EMVideoTransition* p_opTrans)
{
	EMDialogParameters oParams;

	p_opTrans -> GetSettings(reinterpret_cast<int32*>(&oParams));
	SetSettings(reinterpret_cast<int32*>(&oParams));

	return true;
}


bool EMWinChromaVideoTransition::LoadData(EMProjectDataLoader* p_opLoader) 
{ 
	EMDialogParameters oParams;
	char* vpData = reinterpret_cast<char*>(&oParams);

	string oTest = p_opLoader -> LoadString();

	EMWinDirectXVideoTransition::LoadData(p_opLoader);

	int32 vSize = sizeof(EMDialogParameters);
	p_opLoader -> LoadBytes(vpData, vSize);
	SetSettings(reinterpret_cast<int32*>(vpData));

	oTest = p_opLoader -> LoadString();
	
	return true; 
};

bool EMWinChromaVideoTransition::SaveData(EMProjectDataSaver* p_opSaver) 
{ 
	EMDialogParameters oParams;
	char* vpData = reinterpret_cast<char*>(&oParams);

	p_opSaver -> SaveString("EMWinChromaVideoTransition::SaveData - begin");

	EMWinDirectXVideoTransition::SaveData(p_opSaver);

	int32 vSize = sizeof(EMDialogParameters);
	GetSettings(reinterpret_cast<int32*>(vpData));

	p_opSaver -> SaveBytes(vpData, vSize);
	p_opSaver -> SaveString("EMWinChromaVideoTransition::SaveData - end");


	return true; 
};

bool EMWinChromaVideoTransition::LoadSetting(EMProjectDataLoader* p_opLoader)
{
	return LoadData(p_opLoader);
}

bool EMWinChromaVideoTransition::SaveSetting(EMProjectDataSaver* p_opSaver)
{
	return SaveData(p_opSaver);
}

void EMWinChromaVideoTransition::SetSettings(int32* p_vpSettings)
{
	uint8 vR;
	uint8 vG;
	uint8 vB;

	EMDialogParameters* opParams = reinterpret_cast<EMDialogParameters*>(p_vpSettings);

	if(opParams -> m_vID == EM_TRANSITION_KEY)
	{
		m_vSwap = !(opParams -> m_vSwap);
		
		switch(opParams -> m_vParams.m_vKey.m_vColorID)
		{
		case EM_KEY_COLOR:
			m_opIDxt -> put_KeyType(DXTKEY_RGB);

			vR = opParams -> m_vParams.m_vKey.m_vR;
			vG = opParams -> m_vParams.m_vKey.m_vG;
			vB = opParams -> m_vParams.m_vKey.m_vB;

			m_opIDxt -> put_RGB((vR<<16) + (vG<<8) + vB);
			break;
		case EM_KEY_HUE:
			m_opIDxt -> put_KeyType(DXTKEY_HUE);

			m_opIDxt -> put_Hue(opParams -> m_vParams.m_vKey.m_vHue);
			break;
		case EM_KEY_LUMINANCE:
			m_opIDxt -> put_KeyType(DXTKEY_LUMINANCE);

			m_opIDxt -> put_Luminance(opParams -> m_vParams.m_vKey.m_vLuminance);
			break;
		case EM_KEY_BLUE:
			m_opIDxt -> put_KeyType(DXTKEY_NONRED);
			break;
		case EM_KEY_ALPHA:
			m_opIDxt -> put_KeyType(DXTKEY_ALPHA);
			break;
		}

		m_opIDxt -> put_Similarity(opParams -> m_vParams.m_vKey.m_vTolerance);
		m_opIDxt -> put_Invert(opParams -> m_vParams.m_vKey.m_vInvert);
	}
}

void EMWinChromaVideoTransition::GetSettings(int32* p_vpSettings)
{
	uint32 vDummy;
	int vKey;
	EMDialogParameters* opParams = reinterpret_cast<EMDialogParameters*>(p_vpSettings);

	opParams -> m_vID = EM_TRANSITION_KEY;

	m_opIDxt -> get_RGB(&vDummy);
	opParams -> m_vParams.m_vKey.m_vR = (vDummy >> 16) & 0xff;
	opParams -> m_vParams.m_vKey.m_vG = (vDummy >> 8) & 0xff;
	opParams -> m_vParams.m_vKey.m_vB = (vDummy) & 0xff;

	m_opIDxt -> get_Hue(&opParams -> m_vParams.m_vKey.m_vHue);
	m_opIDxt -> get_Luminance(&opParams -> m_vParams.m_vKey.m_vLuminance);

	m_opIDxt -> get_Similarity(&opParams -> m_vParams.m_vKey.m_vTolerance);
	m_opIDxt -> get_Invert(&vKey);

	opParams -> m_vParams.m_vKey.m_vInvert = vKey != 0;
	opParams -> m_vSwap = !m_vSwap;

	m_opIDxt -> get_KeyType(&vKey);

	switch(vKey)
	{
	case DXTKEY_HUE:
		opParams -> m_vParams.m_vKey.m_vColorID = EM_KEY_HUE;
		break;
	case DXTKEY_RGB:
		opParams -> m_vParams.m_vKey.m_vColorID = EM_KEY_COLOR;
		break;
	case DXTKEY_LUMINANCE:
		opParams -> m_vParams.m_vKey.m_vColorID = EM_KEY_LUMINANCE;
		break;
	case DXTKEY_NONRED:
		opParams -> m_vParams.m_vKey.m_vColorID = EM_KEY_BLUE;
		break;
	default:
		opParams -> m_vParams.m_vKey.m_vColorID = EM_KEY_ALPHA;
		break;
	}
}



#endif
