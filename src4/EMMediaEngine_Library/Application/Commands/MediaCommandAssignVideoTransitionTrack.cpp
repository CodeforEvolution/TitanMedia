#include "MediaCommandAssignVideoTransitionTrack.h"
#include "EMMediaTrack.h"
#include "EMMediaVideoTransitionTrack.h"
#include "EMMediaProject.h"
#include "EMMediaEngine.h"
//#include "EMMediaTrackRepository.h"
#include "EMMediaTransitionTrackRepository.h"
#include "EMMediaVideoTransitionTrack.h"

MediaCommandAssignTransitionTrackToVideoTrack::MediaCommandAssignTransitionTrackToVideoTrack()
{
}

EMMediaTrack* MediaCommandAssignTransitionTrackToVideoTrack::GetTrack(uint32 p_vID)
{
//	EMMediaTrackRepository* opUsed = EMMediaEngine::Instance() -> GetMediaProject() -> GetUsedTracks();
//	EMMediaTrackRepository* opFree = EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks();

	EMMediaTransitionTrackRepository* opTrans = EMMediaEngine::Instance() -> GetMediaProject() -> GetVideoTransitionTrackRepository();

/*	EMMediaTrack* opTrack = opUsed -> Find(p_vID);
	if(opTrack == NULL)
	{
		opTrack = opFree -> Find(p_vID);
	}*/

	opTrans -> LockContainer();
	EMMediaVideoTransitionTrack* opTrack = opTrans -> Find(p_vID);
	opTrans -> UnlockContainer(); 

	if(opTrack == NULL)
	{
		EMDebugger("MediaCommandCreateVideoTransitionTrack - Couldn't find track!!");
	}

	return opTrack;
}


void* MediaCommandAssignTransitionTrackToVideoTrack::ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree)
{
	uint32 vTrackID = *(static_cast<uint32*>(p_opParameterOne));
	uint32 vNewParentID = *(static_cast<uint32*>(p_opParameterTwo));

	EMMediaVideoTransitionTrack* opTrans = static_cast<EMMediaVideoTransitionTrack*>(GetTrack(vTrackID));
	EMMediaTrack* opNewTrack = GetTrack(vNewParentID);

	uint32 vOldParentID = opTrans -> GetParentID();
	EMMediaTrack* opOldTrack = GetTrack(vOldParentID);

	opOldTrack -> SetTrackDestination(opTrans -> GetTrackDestination());
	opTrans -> SetTrackDestination(opNewTrack -> GetTrackDestination());
	opNewTrack -> SetTrackDestination(opTrans);
//	opTrans -> SetInput(opNewTrack);

	opTrans -> SetParentID(vNewParentID);

	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);

	return NULL;
}

bool MediaCommandAssignTransitionTrackToVideoTrack::RequiresParameters()
{
	return true;
}

bool MediaCommandAssignTransitionTrackToVideoTrack::IsUndoable()
{
	return false;
}