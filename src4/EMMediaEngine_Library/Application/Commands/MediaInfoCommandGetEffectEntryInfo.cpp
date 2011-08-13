#include "MediaInfoCommandGetEffectEntryInfo.h"

#include "EMPluginRepository.h"
#include "EMPluginEntry.h"

MediaInfoCommandGetEffectEntryInfo::MediaInfoCommandGetEffectEntryInfo()
{
}

void* MediaInfoCommandGetEffectEntryInfo::ExecuteE(void* p_upMediaEffectEntryID, void*, void*)
{
	int32 vMediaEffectEntryID = *static_cast<int32*>(p_upMediaEffectEntryID);
	
	EMPluginEntry* opPluginEntry = NULL;
	EMPluginRepository::Instance() -> LockContainer();
	try
	{
		opPluginEntry = EMPluginRepository::Instance() -> Find(vMediaEffectEntryID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetEffectEntryInfo::ExecuteE");
	}
	EMPluginRepository::Instance() -> UnlockContainer();

	if(opPluginEntry != NULL)
	{
		m_oReturnValue.m_vID = opPluginEntry -> GetID();
		m_oReturnValue.m_eType = opPluginEntry -> GetType();
		m_oReturnValue.m_ePluginType = opPluginEntry -> GetPluginType();
		m_oReturnValue.m_vInstanceCount = opPluginEntry -> GetCount();
		m_oReturnValue.m_oPluginName = opPluginEntry -> GetName();
	}
	else
	{
		EMDebugger("ERROR! Could not locate the effect entry with the specified ID!");
		return NULL;
	}

	return static_cast<void*>(&m_oReturnValue);
}

bool MediaInfoCommandGetEffectEntryInfo::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetEffectEntryInfo::IsUndoable()
{
	return false;
}