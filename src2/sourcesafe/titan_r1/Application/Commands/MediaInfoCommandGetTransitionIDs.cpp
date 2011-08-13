#include "MediaInfoCommandGetTransitionIDs.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMPluginRepository.h"


MediaInfoCommandGetTransitionIDs::MediaInfoCommandGetTransitionIDs()
{
}

void MediaInfoCommandGetTransitionIDs::ExecuteE() {}


void* MediaInfoCommandGetTransitionIDs::ExecuteE(void* p_upIDs, void* p_upOnlyNativeTrans, void*)
{
	int cnt = 0;
	uint32* vpIDs = static_cast<uint32*>(p_upIDs);

	EMPluginRepository *opPlugs = EMPluginRepository::Instance();

	
	opPlugs -> LockContainer();
	opPlugs -> Rewind();
	bool vOnlyNativeTransitions = (p_upOnlyNativeTrans == NULL ? false : *static_cast<bool*>(p_upOnlyNativeTrans));

	while(opPlugs -> Current() != NULL)
	{
		if(vOnlyNativeTransitions)
		{
			if(opPlugs -> Current() -> GetPluginType() == EM_NATIVE_VIDEO_TRANSITION)
			{
				vpIDs[cnt++] = opPlugs -> Current() -> GetID();
			}
		}
		else if(opPlugs -> Current() -> GetPluginType() == EM_WINDOWS_DIRECTX_VIDEO_TRANSITION || opPlugs -> Current() -> GetPluginType() == EM_NATIVE_VIDEO_TRANSITION )
		{
			vpIDs[cnt++] = opPlugs -> Current() -> GetID();
		}
		opPlugs -> Next();
	}

	opPlugs -> UnlockContainer();

	return NULL;
}


bool MediaInfoCommandGetTransitionIDs::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetTransitionIDs::IsUndoable()
{
	return false;
}
