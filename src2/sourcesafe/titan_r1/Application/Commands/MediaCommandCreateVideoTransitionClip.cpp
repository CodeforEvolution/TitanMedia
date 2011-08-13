#include "MediaCommandCreateVideoTransitionclip.h"
#include "EMMediaVideoTransitionTrack.h"
#include "EMMediaTransitionTrackRepository.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"

MediaCommandCreateVideoTransitionClip::MediaCommandCreateVideoTransitionClip()
{
}

MediaCommandCreateVideoTransitionClip::MediaCommandCreateVideoTransitionClip(int32 p_vID)
:  m_vID(p_vID)
{
}

MediaCommandCreateVideoTransitionClip::~MediaCommandCreateVideoTransitionClip()
{
}

void* MediaCommandCreateVideoTransitionClip::ExecuteE(void* p_vpParentTrackID, void* p_vpStartTime, void* p_vpName)
{
	uint32 vParentID = *(static_cast<uint32*>(p_vpParentTrackID));
	uint32 vStart = *(static_cast<uint32*>(p_vpStartTime));
	string* vpName = static_cast<string*>(p_vpName);

/*	EMMediaTrackRepository* opUsed = EMMediaEngine::Instance() -> GetMediaProject() -> GetUsedTracks();
	EMMediaTrackRepository* opFree = EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks();

	opUsed -> LockContainer();
	EMMediaTrack* opTrack = opUsed -> Find(vParentID);
	opUsed -> UnlockContainer();

	if(opTrack == NULL)
	{
		opFree -> LockContainer();
		opTrack = opFree -> Find(vParentID);
		opFree -> UnlockContainer();
	}*/

	EMMediaVideoTransitionTrack* opTrack;
	EMMediaTransitionTrackRepository* opTracks = EMMediaEngine::Instance() -> GetMediaProject() -> GetVideoTransitionTrackRepository();

	opTracks -> LockContainer();
	try
	{
		opTrack = opTracks -> Find(vParentID);
	}
	catch(...)
	{
		EMDebugger("MediaCommandCreateVideoTransitionClip - Caught an exception!!");
	}
	opTracks -> UnlockContainer();

	if(opTrack == NULL)
	{
		EMDebugger("MediaCommandCreateVideoTransitionClip - Couldn't find host track!!");
	}

	m_vID = opTrack -> CreateClip(vStart,"Transition", NULL);

	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
	
	return (void*)&m_vID;
}

bool MediaCommandCreateVideoTransitionClip::RequiresParameters()
{
	return true;
}

bool MediaCommandCreateVideoTransitionClip::IsUndoable()
{
	return false;
}

void MediaCommandCreateVideoTransitionClip::UndoE()
{
	EMDebugger("MediaCommandCreateVideoTransitionClip::UndoE() - Not Implemented yet");
}

EMCommand* MediaCommandCreateVideoTransitionClip::CloneAndClear()
{
	return EM_new MediaCommandCreateVideoTransitionClip(m_vID);
}