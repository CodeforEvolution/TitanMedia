#include "MediaCommandSetTransition.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaVideoTransitionClip.h"
#include "EMWinVideoTransitionClipRepository.h"
#include "EMPluginRepository.h"



MediaCommandSetTransition::MediaCommandSetTransition()
  : m_vClipID(-1),
    m_vOldTransID(-1)
{
}


MediaCommandSetTransition::MediaCommandSetTransition(int32 p_vClipID, int32 p_vOldTransID)
  : m_vClipID(p_vClipID),
    m_vOldTransID(p_vOldTransID)
{
}

void* MediaCommandSetTransition::ExecuteE(void* p_upMediaClipID, void* p_upTransID, void*)
{
	int32 vClipID   = *(static_cast<int32*>(p_upMediaClipID));
	int32 vTransID = *(static_cast<int32*>(p_upTransID));

	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();
 
	EMMediaClipRepository* opVideoTransitionClipRepository = opProject -> GetVideoTransitionClipRepository();

	EMVideoTransitionClip* opClip;

	opVideoTransitionClipRepository -> LockContainer();
	try
	{
		opClip = static_cast<EMVideoTransitionClip*>(opVideoTransitionClipRepository -> Find(vClipID));
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaCommandSetTransition::ExecuteE");
	}
	opVideoTransitionClipRepository -> UnlockContainer();

	m_vClipID = static_cast<EMMediaClip*>(opClip) -> GetID();

	if(opClip -> GetTransition() == NULL)
	{
		m_vOldTransID = -1;
	} 
	else
	{
		m_vOldTransID= opClip -> GetTransition() -> GetID();
	}

	opClip -> SetTransition(EMPluginRepository::Instance() -> Find(vTransID));
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);

	return NULL;
}

bool MediaCommandSetTransition::RequiresParameters()
{
	return true;
}

bool MediaCommandSetTransition::IsUndoable()
{
	return false;
}

void MediaCommandSetTransition::UndoE()
{
	ExecuteE((void*)&m_vClipID, (void*)&m_vOldTransID, NULL);
}

EMCommand* MediaCommandSetTransition::CloneAndClear()
{
	return EM_new MediaCommandSetTransition(m_vClipID, m_vOldTransID);
}
