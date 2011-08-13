#include "EMSimpleVideoEffectDestination.h"

EMSimpleVideoEffectDestination::EMSimpleVideoEffectDestination()  
	:	EMBufferDestination(EM_OUTPUT_TYPE_PREEFFECTS, EM_CLIP, string("EMSimpleVideoEffectDestination"))
{
	m_opEffect = 0;
}

EMSimpleVideoEffectDestination::EMSimpleVideoEffectDestination(EMVideoEffectFilter *p_opEffect)
	:	EMBufferDestination(EM_OUTPUT_TYPE_PREEFFECTS, EM_CLIP, string("EMSimpleVideoEffectDestination"))
{
	m_opEffect = p_opEffect;
}

EMSimpleVideoEffectDestination::~EMSimpleVideoEffectDestination()
{
	if (m_opEffect != 0)
		delete m_opEffect;
}



bool EMSimpleVideoEffectDestination::PrepareToPlayE()
{
	return true;	
}
	


EMMediaDataBuffer* EMSimpleVideoEffectDestination::ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList)
{
	EMMediaFormat *opFormat;

	if (p_opBufferList->size() != 1)
		return 0;

	EMMediaDataBuffer *opBuffer = p_opBufferList->front();
	p_opBufferList->pop_front();

	opFormat = &(opBuffer->m_oFormat);

	m_opEffect -> InitFilter( opFormat->m_vWidth, opFormat->m_vHeight, opFormat->m_vDepth, (opFormat->m_vBytesPerRow - opFormat->m_vDepth*opFormat->m_vWidth), 2.0f);

	if (m_opEffect -> NeedBackBuffer())
	{
		m_opEffect -> Filter(opBuffer -> Data(), opBuffer -> BackBuffer());
		opBuffer -> SwapBuffer();
	} 
	else
	{
		m_opEffect -> Filter(opBuffer -> Data(), opBuffer -> Data());
	}

	opBuffer -> m_vOriginalHasBeenWasted = true;

	return opBuffer;
}