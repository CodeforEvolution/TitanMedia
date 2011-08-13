#include "EMAudioClipMixer.h"
#include "EMMediaDataBuffer.h"
#include "EMAudioMixFunctions.h"
#include "EMBeMediaUtility.h"

EMAudioClipMixer::EMAudioClipMixer(int32 p_vParentId) 
	:	EMBufferDestination(EM_OUTPUT_TYPE_PREEFFECTS, EM_CLIP, string("EMAudioClipMixer")),
		m_fpMixFunction(NULL),
		m_vParentId(p_vParentId)
{
}

EMAudioClipMixer::~EMAudioClipMixer()
{
}

int32 EMAudioClipMixer::GetParentId()
{
	return m_vParentId;
}

bool EMAudioClipMixer::PrepareToPlayE()
{
	//Set mixer-function hooks
	if(! IsPreFader())
		m_fpMixFunction = EMAudioMixFunctions::AudioMixPreFader; 	
	else
		m_fpMixFunction = EMAudioMixFunctions::AudioMixPostFader;
	return true;
}

EMMediaDataBuffer* EMAudioClipMixer::ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList)
{
	if(p_opBufferList -> size() == 0)
		return NULL;
	if(m_fpMixFunction == NULL)
		EMDebugger("ERROR! No mixing-function set in AudioClipMixer!");
	EMMediaDataBuffer* opResult = (*m_fpMixFunction)(p_opBufferList);
	opResult -> m_opDestination = GetDestination();
	return opResult;
}

EMMediaDataBuffer* EMAudioClipMixer::ProcessBufferE(EMMediaDataBuffer** p_opBufferList)
{
/*	if(p_opBufferList[0] != NULL)
	{
		eo << "ERROR! EMAudioClipMixer::ProcessBufferE() - Cant process empty list!" << ef;
		return NULL;
	}
	if(m_fpMixFunction == NULL)
		EMDebugger("ERROR! No mixing-function set in AudioClipMixer!");
	EMMediaDataBuffer* opResult = (*m_fpMixFunction)(p_opBufferList);
	opResult -> m_opDestination = GetDestination(); */
	return NULL;
}

