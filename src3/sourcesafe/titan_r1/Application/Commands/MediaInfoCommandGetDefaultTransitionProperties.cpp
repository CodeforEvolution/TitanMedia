#include "MediaInfoCommandGetDefaultTransitionProperties.h"
#include "EMpluginEntry.h"
#include "EMpluginRepository.h"

MediaInfoCommandGetDefaultTransitionProperties::MediaInfoCommandGetDefaultTransitionProperties()
{
}

void* MediaInfoCommandGetDefaultTransitionProperties::ExecuteE(void* p_upTransID, void* p_upPropertyArray, void*)
{
	int32* vpPropertyArray = static_cast<int32*>(p_upPropertyArray);

	int32  vID = *(static_cast<int32*>(p_upTransID));

	EMPluginEntry* opEntry =   EMPluginRepository::Instance() -> Find(vID);

	if(opEntry != NULL)
	{
		memcpy(vpPropertyArray, opEntry -> GetDefaultProperties(), 10*sizeof(int32));
	}

	return vpPropertyArray;
}

bool MediaInfoCommandGetDefaultTransitionProperties::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetDefaultTransitionProperties::IsUndoable()
{
	return false;
}

void MediaInfoCommandGetDefaultTransitionProperties::UndoE()
{

}

EMCommand* MediaInfoCommandGetDefaultTransitionProperties::CloneAndClear()
{
	return EM_new MediaInfoCommandGetDefaultTransitionProperties();
}
