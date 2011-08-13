#include "EMNoTransition.h"
#include "EMMediaDataBuffer.h"

#include "EMMediaTransitionDialogIDs.h"

EMNoTransition::EMNoTransition() 
	:	EMVideoTransition("EMNoTransition", EM_NATIVE_VIDEO_TRANSITION)
{
	memset(&m_oDefaultParams, 0, sizeof(m_oDefaultParams));
	m_oDefaultParams.m_vID = EM_TRANSITION_FADER;		//<-- Ändra!!
	m_oDefaultParams.m_vSwap = false;
}

EMNoTransition::~EMNoTransition()
{
}

bool EMNoTransition::PrepareToPlayE()
{
	return true;
}


EMMediaDataBuffer* EMNoTransition::ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList)
{
	if(p_opBufferList -> size() != 2)
		return NULL;

	EMMediaDataBuffer* opSource = p_opBufferList -> front();
	p_opBufferList -> pop_front();

	EMMediaDataBuffer* opTarget = p_opBufferList -> front();
	p_opBufferList -> pop_front();

	if(m_vSwap)
	{
		swap(opTarget, opSource);
	}

	opTarget -> Recycle();

	opSource -> m_opDestination = EMBufferDestination::GetDestination();

	return opSource;
}


EMPlugin* EMNoTransition::Clone()
{
	return new EMNoTransition();
}

void EMNoTransition::SetSettings(int32* p_vpSettings)
{
	EMDialogParameters* opParams = reinterpret_cast<EMDialogParameters*>(p_vpSettings);

	if(opParams -> m_vID == EM_TRANSITION_FADER)
		m_vSwap = opParams -> m_vSwap;
}

void EMNoTransition::GetSettings(int32* p_vpSettings)
{
	EMDialogParameters* opParams = reinterpret_cast<EMDialogParameters*>(p_vpSettings);

	opParams -> m_vID = EM_TRANSITION_FADER;
	opParams -> m_vSwap = m_vSwap;
}

void* EMNoTransition::GetDefaultProperties()
{
	return &m_oDefaultParams;
}

bool EMNoTransition::CopyTransParams(EMVideoTransition* p_opTrans)
{
	EMNoTransition* opTrans = dynamic_cast<EMNoTransition*>(p_opTrans);

	m_vSwap = opTrans -> m_vSwap;	

	return true;
}

bool EMNoTransition::LoadData(EMProjectDataLoader* p_opLoader) 
{ 
	EMDialogParameters oParams;
	char* vpData = reinterpret_cast<char*>(&oParams);

	string oTest = p_opLoader -> LoadString();

	EMVideoTransition::LoadData(p_opLoader);

	int32 vSize = sizeof(EMDialogParameters);
	p_opLoader -> LoadBytes(vpData, vSize);
	SetSettings(reinterpret_cast<int32*>(vpData));

	oTest = p_opLoader -> LoadString();
	
	return true; 
};

bool EMNoTransition::SaveData(EMProjectDataSaver* p_opSaver) 
{ 
	EMDialogParameters oParams;
	char* vpData = reinterpret_cast<char*>(&oParams);

	p_opSaver -> SaveString("EMNoTransition::SaveData - begin");

	EMVideoTransition::SaveData(p_opSaver);

	int32 vSize = sizeof(EMDialogParameters);
	GetSettings(reinterpret_cast<int32*>(vpData));
	p_opSaver -> SaveBytes(vpData, vSize);

	p_opSaver -> SaveString("EMNoTransition::SaveData - end");

	return true; 
};

bool EMNoTransition::LoadSetting(EMProjectDataLoader* p_opLoader)
{
	return LoadData(p_opLoader);
}

bool EMNoTransition::SaveSetting(EMProjectDataSaver* p_opSaver)
{
	return SaveData(p_opSaver);
}
