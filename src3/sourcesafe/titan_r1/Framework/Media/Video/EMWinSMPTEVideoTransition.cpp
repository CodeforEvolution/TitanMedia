#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMWinSMPTEVideoTransition.h"
#include "EMWinVideoShuffleOutputFilter.h"
#include "EMMediaSystemInspector.h"
#include "EMPlugin.h"

#include "EMMediaTransitionDialogIDs.h"

#include <streams.h>	


EMWinSMPTEVideoTransition::	EMWinSMPTEVideoTransition(string p_oPluginName, IDirectDraw7* p_opIDD, IMoniker* p_opIMoniker, int p_vSMPTECode)
	:	EMWinDirectXVideoTransition(p_oPluginName, p_opIDD, p_opIMoniker),
	m_vSMPTECode(p_vSMPTECode),
	m_opIDxt(NULL)
{
}

EMWinSMPTEVideoTransition::~EMWinSMPTEVideoTransition()
{
	if (m_opIDxt != NULL)
	{
		m_opIDxt -> Release();
		m_opIDxt = NULL;
	}
}

void EMWinSMPTEVideoTransition::GetDefaultSettings(int32* p_opParam)
{
	memset(p_opParam, 0, sizeof(EMDialogParameters));

	reinterpret_cast<EMDialogParameters*>(p_opParam) -> m_vID = EM_TRANSITION_WIPE;
	reinterpret_cast<EMDialogParameters*>(p_opParam) -> m_vParams.m_vWipe.m_vReplicateX = 1;
	reinterpret_cast<EMDialogParameters*>(p_opParam) -> m_vParams.m_vWipe.m_vReplicateY = 1;
	reinterpret_cast<EMDialogParameters*>(p_opParam) -> m_vSwap = false;
}

bool EMWinSMPTEVideoTransition::UpdateSettings()
{
	HRESULT hr;

	EMDialogParameters oParam;

	if(m_opIDxt != NULL)
	{
//		GetSettings(reinterpret_cast<int32*>(&oParam));
		oParam = m_oParams;
		m_opIDxt -> Release();
		m_opIDxt = NULL;
	}
	else
		GetDefaultSettings(reinterpret_cast<int32*>(&oParam));

	hr = m_opIEffect -> QueryInterface(IID_IDxtJpeg, (void**)&m_opIDxt);
	if(hr != S_OK)
		return false;

	hr = m_opIDxt -> put_MaskNum(m_vSMPTECode);
	if(hr != S_OK)
		return false;

	SetSettings(reinterpret_cast<int32*>(&oParam));

	return true;
}

bool EMWinSMPTEVideoTransition::NeedReset()
{
	return false;
}

void EMWinSMPTEVideoTransition::SetBorderWidth(long p_vParam)
{
	if (m_opIDxt != NULL)
		m_opIDxt -> put_BorderWidth(p_vParam);
}

void EMWinSMPTEVideoTransition::SetBorderColor(long p_vParam)
{
	if (m_opIDxt != NULL)
		m_opIDxt -> put_BorderColor(p_vParam);
}

void EMWinSMPTEVideoTransition::SetBorderSoftness(long p_vParam)
{
	if (m_opIDxt != NULL)
		m_opIDxt -> put_BorderSoftness(p_vParam);
}

void EMWinSMPTEVideoTransition::SetSMPTECode(long p_vParam)
{
	if (m_opIDxt != NULL)
		m_opIDxt -> put_MaskNum(p_vParam);
}


long EMWinSMPTEVideoTransition::GetBorderWidth()
{
	long vParam = -1;

	if (m_opIDxt != NULL)
		m_opIDxt -> get_BorderWidth(&vParam);

	return vParam;

}

long EMWinSMPTEVideoTransition::GetBorderColor()
{
	long vParam = -1;

	if (m_opIDxt != NULL)
		m_opIDxt -> get_BorderColor(&vParam);

	return vParam;

}

long EMWinSMPTEVideoTransition::GetBorderSoftness()
{
	long vParam = -1;

	if (m_opIDxt != NULL)
		m_opIDxt -> get_BorderSoftness(&vParam);

	return vParam;

}

long EMWinSMPTEVideoTransition::GetSMPTECode()
{
	long vParam = -1;

	if (m_opIDxt != NULL)
		m_opIDxt -> get_MaskNum(&vParam);

	return vParam;

}


bool EMWinSMPTEVideoTransition::CopyTransParams(EMVideoTransition* p_opTrans)
{
	EMDialogParameters oParams;

	p_opTrans -> GetSettings(reinterpret_cast<int32*>(&oParams));
	SetSettings(reinterpret_cast<int32*>(&oParams));

	return true;
}


bool EMWinSMPTEVideoTransition::LoadData(EMProjectDataLoader* p_opLoader) 
{ 
	EMDialogParameters oParams;
	char* vpData = reinterpret_cast<char*>(&oParams);

	string oTest = p_opLoader -> LoadString();

	EMWinDirectXVideoTransition::LoadData(p_opLoader);

	m_vSMPTECode = p_opLoader -> LoadInt32();

	int32 vSize = sizeof(EMDialogParameters);

	p_opLoader -> LoadBytes(vpData, vSize);

	SetSettings(reinterpret_cast<int32*>(vpData));

	oTest = p_opLoader -> LoadString();
	
	return true; 
};

bool EMWinSMPTEVideoTransition::SaveData(EMProjectDataSaver* p_opSaver) 
{ 
	EMDialogParameters oParams;
	char* vpData = reinterpret_cast<char*>(&oParams);

	p_opSaver -> SaveString("EMWinSMPTEVideoTransition::SaveData - begin");

	EMWinDirectXVideoTransition::SaveData(p_opSaver);

	p_opSaver -> SaveInt32(m_vSMPTECode);

	int32 vSize = sizeof(EMDialogParameters);

	GetSettings(reinterpret_cast<int32*>(vpData));

	p_opSaver -> SaveBytes(vpData, vSize);

	p_opSaver -> SaveString("EMWinSMPTEVideoTransition::SaveData - end");

	return true; 
};

bool EMWinSMPTEVideoTransition::LoadSetting(EMProjectDataLoader* p_opLoader)
{
	return LoadData(p_opLoader);
}

bool EMWinSMPTEVideoTransition::SaveSetting(EMProjectDataSaver* p_opSaver)
{
	return SaveData(p_opSaver);
}

void EMWinSMPTEVideoTransition::SetSettings(int32* p_vpSettings)
{
	EMDialogParameters* opParams = reinterpret_cast<EMDialogParameters*>(p_vpSettings);
	m_oParams = *opParams;

	if(opParams -> m_vID == EM_TRANSITION_WIPE)
	{
		m_vSwap = opParams -> m_vSwap;

		uint8 vR = opParams -> m_vParams.m_vWipe.m_vR;
		uint8 vG = opParams -> m_vParams.m_vWipe.m_vG;
		uint8 vB = opParams -> m_vParams.m_vWipe.m_vB;

		m_opIDxt -> put_BorderColor((vR<<16) + (vG<<8) + vB);

		m_opIDxt -> put_BorderSoftness(opParams -> m_vParams.m_vWipe.m_vSoftness);
		m_opIDxt -> put_BorderWidth(opParams -> m_vParams.m_vWipe.m_vWidth);
		m_opIDxt -> put_OffsetX(opParams -> m_vParams.m_vWipe.m_vOffsetX);
		m_opIDxt -> put_OffsetY(opParams -> m_vParams.m_vWipe.m_vOffsetY);
		m_opIDxt -> put_ReplicateX(opParams -> m_vParams.m_vWipe.m_vReplicateX);
		m_opIDxt -> put_ReplicateY(opParams -> m_vParams.m_vWipe.m_vReplicateY);
	}
}

void EMWinSMPTEVideoTransition::GetSettings(int32* p_vpSettings)
{
	int32 vColor;
	EMDialogParameters* opParams = reinterpret_cast<EMDialogParameters*>(p_vpSettings);

	opParams -> m_vID = EM_TRANSITION_WIPE;

	m_opIDxt -> get_BorderColor(&vColor);

	opParams -> m_vParams.m_vWipe.m_vR = (vColor >> 16) & 0xff;
	opParams -> m_vParams.m_vWipe.m_vG = (vColor >> 8) & 0xff;
	opParams -> m_vParams.m_vWipe.m_vB = (vColor) & 0xff;

	m_opIDxt -> get_BorderSoftness(&opParams -> m_vParams.m_vWipe.m_vSoftness);
	m_opIDxt -> get_BorderWidth(&opParams -> m_vParams.m_vWipe.m_vWidth);
	m_opIDxt -> get_OffsetX(&opParams -> m_vParams.m_vWipe.m_vOffsetX);
	m_opIDxt -> get_OffsetY(&opParams -> m_vParams.m_vWipe.m_vOffsetY);
	m_opIDxt -> get_ReplicateX(&opParams -> m_vParams.m_vWipe.m_vReplicateX);
	m_opIDxt -> get_ReplicateY(&opParams -> m_vParams.m_vWipe.m_vReplicateY);

	opParams -> m_vSwap = m_vSwap;
}



#endif
