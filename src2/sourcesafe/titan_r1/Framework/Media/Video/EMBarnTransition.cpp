#include "EMBarnTransition.h"
#include "EMMediaDataBuffer.h"

#include "EMMediaTransitionDialogIDs.h"

EMBarnTransition::EMBarnTransition() 
	:	EMVideoTransition("EMBarnTransition", EM_NATIVE_VIDEO_TRANSITION)
{
	memset(&m_oDefaultParams, 0, sizeof(m_oDefaultParams));
	m_oDefaultParams.m_vID = EM_TRANSITION_FADER;		//<-- Ändra!!
	m_oDefaultParams.m_vSwap = false;
}

EMBarnTransition::~EMBarnTransition()
{
}

bool EMBarnTransition::PrepareToPlayE()
{
	return true;
}


EMMediaDataBuffer* EMBarnTransition::ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList)
{
	int x, y, n;
	unsigned char *pPtrSrc1;
	unsigned char *pPtrSrc2;
	unsigned char *pPtrDst;
	float vScale;

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

	pPtrSrc1 = static_cast<unsigned char *>(opSource->Data());
	pPtrSrc2 = static_cast<unsigned char *>(opTarget->Data());

	pPtrDst  = static_cast<unsigned char *>(opSource->BackBuffer());

	vScale = CalcTime(opSource) / 2.0f; // -> GetRelativeTimeToStart());

	int vSwitch1 =(0.5f -  vScale) * opTarget->m_oFormat.m_vWidth;
	int vSwitch2 =(0.5f +  vScale) * opTarget->m_oFormat.m_vWidth;

	for(y = 0; y < opTarget->m_oFormat.m_vHeight; ++y)
	{
		for(x = 0; x < opTarget->m_oFormat.m_vWidth; ++x)
		{
			for(n = 0; n < opTarget->m_oFormat.m_vBytesPerSample; ++n)
			{
				*pPtrDst = (x < vSwitch1 || x > vSwitch2) ? *pPtrSrc1 : *pPtrSrc2;

				++pPtrDst;
				++pPtrSrc1;
				++pPtrSrc2;
			}
		}
	}

	opTarget -> Recycle();

	opSource -> m_opDestination = EMBufferDestination::GetDestination();

	opSource -> SwapBuffer();

	return opSource;
}


EMPlugin* EMBarnTransition::Clone()
{
	return new EMBarnTransition();
}

void EMBarnTransition::SetSettings(int32* p_vpSettings)
{
	EMDialogParameters* opParams = reinterpret_cast<EMDialogParameters*>(p_vpSettings);

	if(opParams -> m_vID == EM_TRANSITION_FADER)
		m_vSwap = opParams -> m_vSwap;
}

void EMBarnTransition::GetSettings(int32* p_vpSettings)
{
	EMDialogParameters* opParams = reinterpret_cast<EMDialogParameters*>(p_vpSettings);

	opParams -> m_vID = EM_TRANSITION_FADER;
	opParams -> m_vSwap = m_vSwap;
}

void* EMBarnTransition::GetDefaultProperties()
{
	return &m_oDefaultParams;
}

bool EMBarnTransition::CopyTransParams(EMVideoTransition* p_opTrans)
{
	EMBarnTransition* opTrans = dynamic_cast<EMBarnTransition*>(p_opTrans);

	m_vSwap = opTrans -> m_vSwap;	

	return true;
}

bool EMBarnTransition::LoadData(EMProjectDataLoader* p_opLoader) 
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

bool EMBarnTransition::SaveData(EMProjectDataSaver* p_opSaver) 
{ 
	EMDialogParameters oParams;
	char* vpData = reinterpret_cast<char*>(&oParams);

	p_opSaver -> SaveString("EMBarnTransition::SaveData - begin");

	EMVideoTransition::SaveData(p_opSaver);

	int32 vSize = sizeof(EMDialogParameters);
	GetSettings(reinterpret_cast<int32*>(vpData));
	p_opSaver -> SaveBytes(vpData, vSize);

	p_opSaver -> SaveString("EMBarnTransition::SaveData - end");

	return true; 
};

bool EMBarnTransition::LoadSetting(EMProjectDataLoader* p_opLoader)
{
	return LoadData(p_opLoader);
}

bool EMBarnTransition::SaveSetting(EMProjectDataSaver* p_opSaver)
{
	return SaveData(p_opSaver);
}
