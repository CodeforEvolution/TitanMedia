#include "MediaInfoCommandGetTransition.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaVideoTransitionClip.h"
#include "EMWinVideoTransitionClipRepository.h"
#include "EMPluginRepository.h"


MediaInfoCommandGetTransition::MediaInfoCommandGetTransition()
{
}

void* MediaInfoCommandGetTransition::ExecuteE(void* p_upMediaClipID, void*, void*)
{
	EMVideoTransitionClip* opClip;
	int32 vClipID   = *(static_cast<int32*>(p_upMediaClipID));

	EMMediaClipRepository* opVideoTransitionClipRepository = EMMediaEngine::Instance() -> GetMediaProject() -> GetVideoTransitionClipRepository();

	opVideoTransitionClipRepository -> LockContainer();
	try
	{
		opClip = static_cast<EMVideoTransitionClip*>(opVideoTransitionClipRepository -> Find(vClipID));
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetTransition::ExecuteE");
	}
	opVideoTransitionClipRepository -> UnlockContainer();

	m_vTransID = opClip -> GetTransition() -> GetEntryID();

	return static_cast<void*>(&m_vTransID);
}

bool MediaInfoCommandGetTransition::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetTransition::IsUndoable()
{
	return false;
}
