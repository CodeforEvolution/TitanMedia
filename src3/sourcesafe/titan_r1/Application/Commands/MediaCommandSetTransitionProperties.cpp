#include "MediaCommandSetTransitionProperties.h"
#include "EMplugin.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMWinVideoTransitionClipRepository.h"

MediaCommandSetTransitionProperties::MediaCommandSetTransitionProperties()
{
}

void* MediaCommandSetTransitionProperties::ExecuteE(void* p_upClipID, void* p_upPropertyArray, void*)
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
		static_cast<EMVideoTransition*>(opTrans) -> SetSettings(static_cast<int32*>(p_upPropertyArray));

		EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
	}

	return NULL;
}

bool MediaCommandSetTransitionProperties::RequiresParameters()
{
	return true;
}

bool MediaCommandSetTransitionProperties::IsUndoable()
{
	return false;
}

void MediaCommandSetTransitionProperties::UndoE()
{

}

EMCommand* MediaCommandSetTransitionProperties::CloneAndClear()
{
	return EM_new MediaCommandSetTransitionProperties();
}
