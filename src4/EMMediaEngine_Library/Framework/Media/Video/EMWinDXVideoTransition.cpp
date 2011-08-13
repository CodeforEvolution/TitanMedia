#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMWinDXVideoTransition.h"
#include "EMWinVideoShuffleOutputFilter.h"
#include "EMMediaSystemInspector.h"
#include "EMPlugin.h"

#include "EMMediaTransitionDialogIDs.h"

#include <streams.h>	


EMWinDXVideoTransition::EMWinDXVideoTransition(string p_oPluginName, IDirectDraw7* p_opIDD, IMoniker* p_opIMoniker)
	:	EMWinDirectXVideoTransition(p_oPluginName, p_opIDD, p_opIMoniker)
{
	m_vBackup = false;
}

EMWinDXVideoTransition::~EMWinDXVideoTransition()
{
}


bool EMWinDXVideoTransition::CopyTransParams(EMVideoTransition* p_opTrans)
{
	EMWinDXVideoTransition* opTrans = static_cast<EMWinDXVideoTransition*>(p_opTrans);

	m_vSwap = opTrans -> m_vSwap;

	return true;
}


bool EMWinDXVideoTransition::LoadData(EMProjectDataLoader* p_opLoader) 
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

bool EMWinDXVideoTransition::SaveData(EMProjectDataSaver* p_opSaver) 
{ 
	EMDialogParameters oParams;
	char* vpData = reinterpret_cast<char*>(&oParams);

	p_opSaver -> SaveString("EMWinDXVideoTransition::SaveData - begin");

	EMWinDirectXVideoTransition::SaveData(p_opSaver);

	int32 vSize = sizeof(EMDialogParameters);
	GetSettings(reinterpret_cast<int32*>(vpData));
	p_opSaver -> SaveBytes(vpData, vSize);

	p_opSaver -> SaveString("EMWinDXVideoTransition::SaveData - end");

	return true; 
};

bool EMWinDXVideoTransition::LoadSetting(EMProjectDataLoader* p_opLoader)
{
	return LoadData(p_opLoader);
}

bool EMWinDXVideoTransition::SaveSetting(EMProjectDataSaver* p_opSaver)
{
	return SaveData(p_opSaver);
}

void EMWinDXVideoTransition::SetSettings(int32* p_vpSettings)
{
	EMDialogParameters* opParams = reinterpret_cast<EMDialogParameters*>(p_vpSettings);

	if(opParams -> m_vID == EM_TRANSITION_DX)
		m_vSwap = opParams -> m_vSwap;
}

void EMWinDXVideoTransition::GetSettings(int32* p_vpSettings)
{
	EMDialogParameters* opParams = reinterpret_cast<EMDialogParameters*>(p_vpSettings);

	opParams -> m_vID = EM_TRANSITION_DX;
	opParams -> m_vSwap = m_vSwap; 
}

bool EMWinDXVideoTransition::UpdateSettings()
{
	return true;
}

float EMWinDXVideoTransition::CalcTime(EMMediaDataBuffer* p_opBuffer)
{
	return EMVideoTransition::CalcTime(p_opBuffer);
}

bool EMWinDXVideoTransition::NeedReset()
{
	return true;
}
#endif
