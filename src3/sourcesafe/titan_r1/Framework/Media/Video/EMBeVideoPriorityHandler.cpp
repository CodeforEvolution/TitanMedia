#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#include "EMBeVideoPriorityHandler.h"
#include "EMMediaDataBuffer.h"
#include "EMBeMixFunctions.h"
#include "EMBeMediaUtility.h"

#include <iostream>

EMBeVideoPriorityHandler::EMBeVideoPriorityHandler() 
	:	EMBufferDestination(EM_OUTPUT_TYPE_PREEFFECTS, EM_TRACK),
		m_fpMixFunction(NULL)
{
	EMBeMediaUtility::push(this, "EMBeVideoPriorityHandler");
}

EMBeVideoPriorityHandler::~EMBeVideoPriorityHandler()
{
	EMBeMediaUtility::pop("EMBeVideoPriorityHandler");
}

bool EMBeVideoPriorityHandler::PrepareToPlayE()
{
	//Set mixer-function hooks
/*	if(!IsPreFader())
		m_fpMixFunction = EMBeMixFunctions::AudioMixPreFader; 	
	else
		m_fpMixFunction = EMBeMixFunctions::AudioMixPostFader;
*/
	m_fpMixFunction = EMBeMixFunctions::VideoPrioritizer;
	return true;
}

EMMediaDataBuffer* EMBeVideoPriorityHandler::ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList)
{
	if(p_opBufferList -> size() <= 0)
		return NULL;
	
	if(m_fpMixFunction == NULL)
		EMDebugger("ERROR! No mixing-function set in VideoClipMixer!");
	EMMediaDataBuffer* opResult = (*m_fpMixFunction)(p_opBufferList);
	opResult -> m_opDestination = m_opDestination;
	return opResult;
}

#endif
