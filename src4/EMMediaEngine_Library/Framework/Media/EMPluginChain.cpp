#include "EMPluginChain.h"

#include "EMGlobals.h"
#include "EMMediaIDManager.h"
#include "EMMediaDataBuffer.h"
#include "EMAudioMixFunctions.h"
#include "EMWinDirectXPluginWrapperInterface.h"

EMPluginChain::EMPluginChain(EMMediaType p_vMediaType, EMPluginType p_ePluginType)
	:	EMMediaItemContainer<EMPlugin>(p_vMediaType),
		EMBufferDestination(EM_OUTPUT_TYPE_EFFECTSLOT, EM_EFFECT),
		m_vType(p_vMediaType),
		m_ePluginType(p_ePluginType),
		m_vID(-1),
		m_vIsInitialized(false),
		m_opDirectXPluginInterface(NULL)
{
	m_vID = EMMediaIDManager::MakeID();
}

EMPluginChain::~EMPluginChain()
{
}

EMMediaType EMPluginChain::GetType() const
{
	return m_vType;
}

EMPluginType EMPluginChain::GetPluginType() const
{
	return m_ePluginType;
}

int32 EMPluginChain::GetID() const
{
	return m_vID;
}

/*bool EMPluginChain::InsertPluginBefore(EMPlugin* p_opPlugin, int32 p_vReferencePluginID)
{
	return true;
}

bool EMPluginChain::AddPluginLast(EMPlugin* p_opPlugin)
{
	return true;
}*/


bool EMPluginChain::PrepareToPlayE()
{
	//Set mixer-function hooks
	if(! IsPreFader())
		m_fpMixFunction = EMAudioMixFunctions::AudioMixPreFader; 	
	else
		m_fpMixFunction = EMAudioMixFunctions::AudioMixPostFader;
	return true;
}


EMMediaDataBuffer* EMPluginChain::ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList)
{
	if(p_opBufferList -> size() <= 0)
	{
		eo << "ERROR! EMWinDirectXPlugin::ProcessBufferE() - Cant process empty list!" << ef;
		return NULL;
	}
	
	if(m_fpMixFunction == NULL)
		EMDebugger("ERROR! No mixing-function set in AudioClipMixer!");
	EMMediaDataBuffer* opResult = (*m_fpMixFunction)(p_opBufferList);

	//Ok we have the buffer that should be sent into this plugin.. So.. Process it!

	m_opDirectXPluginInterface -> ProcessBuffer(opResult);
	m_opDirectXPluginInterface -> WaitForProcessing(opResult);

	opResult -> m_opDestination = m_opDestination;
	return opResult;
}

void EMPluginChain::SetInitialized()
{
	m_vIsInitialized = true;
}

bool EMPluginChain::IsInitialized() const
{
	return m_vIsInitialized;
}

bool EMPluginChain::AddEffectLast(EMPlugin* p_opPluginToAdd)
{
	if((GetType() & p_opPluginToAdd -> GetType()) <= 0)
	{
		eo << "Chain type = " << (int32) GetType() << ef;
		eo << "Plug type  = " << (int32) p_opPluginToAdd -> GetType() << ef;
		EMDebugger("ERROR! The plugin to be added has a wrong media type for this plugin chain!");
		return false;
	}

	if(GetPluginType() != p_opPluginToAdd -> GetPluginType())
	{
		EMDebugger("ERROR! The plugin to be added has a wrong plugin type for this plugin chain!");
		return false;
	}

	EMPlugin* opLastPlugin = NULL;
//	LockContainer();

//	try
//	{
//		eo << "Got lock!" << ef;
		Rewind();
		while(Current() != NULL)
		{
			opLastPlugin = Current();
			Next();
		}
//	}
//	catch(...)
//	{
//		EMDebugger("ERROR! Exception in EMPluginChain::AddEffectLast");
//	}
//	UnlockContainer();
//eo << "Wuha!" << ef;
	if(! PrepareToAddLast(p_opPluginToAdd))
	{
		EMDebugger("ERROR! Failed to prepare for the plugin's addition to the chain!");
		return false;
	}
//	eo << "Returning from here..." << ef;
	return true;
}

bool EMPluginChain::InsertEffectBefore(EMPlugin* p_opPluginToInsert, int32 p_vReferencePluginID)
{
	EMPlugin* opReferencePlugin = NULL;
	LockContainer();
	try
	{
		Rewind();
		while(Current() != NULL)
		{
			if(Current() -> GetID() == p_vReferencePluginID)
			{
				opReferencePlugin = Current();
				break;
			}
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in EMPluginChain::InsertEffectBefore");
	}
	UnlockContainer();

	if(! PrepareToInsertBefore(p_opPluginToInsert, opReferencePlugin))
		return false;
	return true;
}

