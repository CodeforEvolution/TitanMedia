#include "EMVideoClipMixer.h"
#include "EMMediaDataBuffer.h"

#include "EMMediaTransitionDialogIDs.h"

EMVideoClipMixer::EMVideoClipMixer() 
	:	EMVideoTransition("EMVideoClipMixer", EM_NATIVE_VIDEO_TRANSITION)
{
	memset(&m_oDefaultParams, 0, sizeof(m_oDefaultParams));
	m_oDefaultParams.m_vID = EM_TRANSITION_FADER;
	m_oDefaultParams.m_vSwap = false;
}

EMVideoClipMixer::~EMVideoClipMixer()
{
}

bool EMVideoClipMixer::PrepareToPlayE()
{
	return true;
}

/*float EMVideoClipMixer::GetScale(int64 p_vTime)
{
	float vLen = m_vStop - 	m_vStart;
	float vDur = p_vTime - m_vStart;

	vDur /= vLen;

	if(vDur < 0)
		vDur = 0;

	if(vDur > 1)
		vDur = 1;

	return 1.0f - vDur;
}*/


EMMediaDataBuffer* EMVideoClipMixer::ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList)
{
	int i;
	unsigned char *pPtrSrc1;
	unsigned char *pPtrSrc2;
	unsigned char *pPtrDst;
	float vScale;
	float vScale2;

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

	uint64 vMax = opTarget -> m_vSizeUsed;

	pPtrSrc1 = static_cast<unsigned char *>(opSource->Data());
	pPtrSrc2 = static_cast<unsigned char *>(opTarget->Data());

	pPtrDst  = static_cast<unsigned char *>(opSource->BackBuffer());

	vScale2 = CalcTime(opSource); // -> GetRelativeTimeToStart());
	vScale = 1.0f - vScale2;

	for (i = 0; i < vMax; ++i, ++pPtrSrc1, ++pPtrSrc2, ++pPtrDst)
	{
		*pPtrDst = vScale * static_cast<float>(*pPtrSrc1) + vScale2 * static_cast<float>(*pPtrSrc2);
	}

	opTarget -> Recycle();

	opSource -> m_opDestination = EMBufferDestination::GetDestination();

	opSource -> SwapBuffer();

	return opSource;
}


EMPlugin* EMVideoClipMixer::Clone()
{
	return new EMVideoClipMixer();
}

void EMVideoClipMixer::SetSettings(int32* p_vpSettings)
{
	EMDialogParameters* opParams = reinterpret_cast<EMDialogParameters*>(p_vpSettings);

	if(opParams -> m_vID == EM_TRANSITION_FADER)
		m_vSwap = opParams -> m_vSwap;
}

void EMVideoClipMixer::GetSettings(int32* p_vpSettings)
{
	EMDialogParameters* opParams = reinterpret_cast<EMDialogParameters*>(p_vpSettings);

	opParams -> m_vID = EM_TRANSITION_FADER;
	opParams -> m_vSwap = m_vSwap;
}

void* EMVideoClipMixer::GetDefaultProperties()
{
	return &m_oDefaultParams;
}

bool EMVideoClipMixer::CopyTransParams(EMVideoTransition* p_opTrans)
{
	EMVideoClipMixer* opTrans = dynamic_cast<EMVideoClipMixer*>(p_opTrans);

	m_vSwap = opTrans -> m_vSwap;	

	return true;
}

bool EMVideoClipMixer::LoadData(EMProjectDataLoader* p_opLoader) 
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

bool EMVideoClipMixer::SaveData(EMProjectDataSaver* p_opSaver) 
{ 
	EMDialogParameters oParams;
	char* vpData = reinterpret_cast<char*>(&oParams);

	p_opSaver -> SaveString("EMVideoClipMixer::SaveData - begin");

	EMVideoTransition::SaveData(p_opSaver);

	int32 vSize = sizeof(EMDialogParameters);
	GetSettings(reinterpret_cast<int32*>(vpData));
	p_opSaver -> SaveBytes(vpData, vSize);

	p_opSaver -> SaveString("EMVideoClipMixer::SaveData - end");

	return true; 
};

bool EMVideoClipMixer::LoadSetting(EMProjectDataLoader* p_opLoader)
{
	return LoadData(p_opLoader);
}

bool EMVideoClipMixer::SaveSetting(EMProjectDataSaver* p_opSaver)
{
	return SaveData(p_opSaver);
}
