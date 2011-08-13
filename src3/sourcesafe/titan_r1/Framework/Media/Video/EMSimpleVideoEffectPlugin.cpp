#include "EMSimpleVideoEffectPlugin.h"
#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"


EMSimpleVideoEffectPlugin::EMSimpleVideoEffectPlugin()  
	: EMNativePlugin("", EM_TYPE_RAW_VIDEO, EM_NATIVE_VIDEO_EFFECT, EM_EFFECT)
{
	m_opEffect = 0;
}

EMSimpleVideoEffectPlugin::EMSimpleVideoEffectPlugin(EMVideoEffectFilter *p_opEffect)
	: EMNativePlugin(p_opEffect -> GetName(), EM_TYPE_RAW_VIDEO, EM_NATIVE_VIDEO_EFFECT, EM_EFFECT)
{
	m_opEffect = p_opEffect;
}

EMSimpleVideoEffectPlugin::~EMSimpleVideoEffectPlugin()
{
	if (m_opEffect != 0)
		delete m_opEffect;
}



bool EMSimpleVideoEffectPlugin::PrepareToPlayE()
{
	return true;	
}

EMMediaDataBuffer* EMSimpleVideoEffectPlugin::ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList)
{
	EMMediaFormat *opFormat;

	EMMediaDataBuffer *opBuffer = p_opBufferList->front();
	p_opBufferList->pop_front();

	while(p_opBufferList -> size() != 0)
	{
		EMMediaDataBuffer *opBuffer2 = p_opBufferList->front();
		p_opBufferList -> pop_front();

		opBuffer2 -> Recycle();
	}

	opFormat = &(opBuffer -> m_oFormat);

	m_opEffect -> InitFilter(opFormat -> m_vWidth, opFormat -> m_vHeight, opFormat -> m_vDepth, (opFormat -> m_vBytesPerRow - opFormat -> m_vDepth*opFormat -> m_vWidth), 200.0f * (GetDryWetMix() - 0.5f));

	if(m_opEffect -> NeedBackBuffer())
	{
		m_opEffect -> Filter(opBuffer -> Data(), opBuffer -> BackBuffer());
		opBuffer -> SwapBuffer();
	} 
	else
	{
		m_opEffect -> Filter( opBuffer->Data(), opBuffer -> Data());
	}

	opBuffer -> m_vOriginalHasBeenWasted = true;
	opBuffer -> m_opDestination = GetDestination();

	return opBuffer;
}

list<EMPluginPreset*>* EMSimpleVideoEffectPlugin::GetPresets()
{
	return NULL;
}

bool EMSimpleVideoEffectPlugin::ActivatePreset(int32 p_vID)
{
	return false;
}

bool EMSimpleVideoEffectPlugin::LoadSetting(EMProjectDataLoader* p_opLoader)
{
	SetDryWetMix(p_opLoader -> LoadFloat());
	m_opEffect -> LoadSetting(p_opLoader);
	return true;
}

bool EMSimpleVideoEffectPlugin::SaveSetting(EMProjectDataSaver* p_opSaver)
{
	p_opSaver -> SaveFloat(GetDryWetMix());
	m_opEffect -> SaveSetting(p_opSaver);
	return true;
}

void* EMSimpleVideoEffectPlugin::GetNativeEffect() const
{
	return static_cast<void*>(m_opEffect);
}


EMPlugin* EMSimpleVideoEffectPlugin::Clone()
{
	EMPlugin* opPlug = EM_new EMSimpleVideoEffectPlugin(m_opEffect -> Clone());

	opPlug -> SetDryWetMix(GetDryWetMix());

	return opPlug;
}

bool EMSimpleVideoEffectPlugin::ShowDialog()
{
	return m_opEffect -> ShowDialog();
}