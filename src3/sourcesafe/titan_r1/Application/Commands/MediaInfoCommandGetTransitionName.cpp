#include "MediaInfoCommandGetTransitionName.h"
#include "EMPluginRepository.h"
#include "EMPlugin.h"


MediaInfoCommandGetTransitionName::MediaInfoCommandGetTransitionName()
{
}

void MediaInfoCommandGetTransitionName::ExecuteE() 
{
}

void* MediaInfoCommandGetTransitionName::ExecuteE(void* p_upTransID, void*, void*)
{
	int32 vTransID   = *(static_cast<int32*>(p_upTransID));

	EMPluginRepository* opPlugs = EMPluginRepository::Instance();
	EMPluginEntry* opPlug = opPlugs -> Find(vTransID);
	m_vpName = opPlug ->GetName();

	return static_cast<void*>(&m_vpName);
}

bool MediaInfoCommandGetTransitionName::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetTransitionName::IsUndoable()
{
	return false;
}
