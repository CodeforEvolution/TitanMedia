#include "MediaInfoCommandGetTransitionProperties.h"
#include "EMplugin.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMWinVideoTransitionClipRepository.h"

MediaInfoCommandGetTransitionProperties::MediaInfoCommandGetTransitionProperties()
{
}

void* MediaInfoCommandGetTransitionProperties::ExecuteE(void* p_upClipID, void* p_upPropertyArray, void*)
{
	EMMediaClip* opClip = NULL;
	EMPlugin* opTrans = NULL;

	int32  vID = *(static_cast<int32*>(p_upClipID));

	try
	{
		opClip = EMMediaEngine::Instance() -> GetMediaProject() -> GetVideoTransitionClipRepository() -> Find(vID);
	}
	catch(...)
	{
	}

	if(opClip != NULL && opClip -> GetType() == EM_TYPE_TRANSITION)
	{
		opTrans = static_cast<EMVideoTransitionClip*>(opClip) -> GetTransition();
	}

	EMPluginType oType = opTrans -> GetPluginType();

	if(opTrans != NULL &&  (oType == EM_NATIVE_VIDEO_TRANSITION || oType == EM_WINDOWS_DIRECTX_VIDEO_TRANSITION))
	{
		static_cast<EMVideoTransition*>(opTrans) -> GetSettings(static_cast<int32*>(p_upPropertyArray));
	}

	return p_upPropertyArray;

/*	void* upPropertyArray = NULL;

	int32  vID = *(static_cast<int32*>(p_upClipID));

	EMPluginEntry* opEntry =   EMPluginRepository::Instance() -> Find(vID);

	if(opEntry != NULL)
	{
		upPropertyArray = opEntry -> GetPersistProperties();
	}*/

	return p_upPropertyArray;
}

bool MediaInfoCommandGetTransitionProperties::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetTransitionProperties::IsUndoable()
{
	return false;
}

void MediaInfoCommandGetTransitionProperties::UndoE()
{

}

EMCommand* MediaInfoCommandGetTransitionProperties::CloneAndClear()
{
	return EM_new MediaInfoCommandGetTransitionProperties();
}
