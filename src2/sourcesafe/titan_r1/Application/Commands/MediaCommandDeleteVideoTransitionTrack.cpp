#include "MediaCommandDeleteVideoTransitionTrack.h"
#include "EMMediaTrack.h"
#include "EMMediaVideoTransitionTrack.h"
#include "EMMediaProject.h"
#include "EMMediaEngine.h"
#include "EMMediaTrackRepository.h"

MediaCommandDeleteVideoTransitionTrack::MediaCommandDeleteVideoTransitionTrack()
{
}



void* MediaCommandDeleteVideoTransitionTrack::ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree)
{
	uint32 vID = *(static_cast<uint32*>(p_opParameterOne));

	EMMediaVideoTransitionTrack* opTrans = static_cast<EMMediaVideoTransitionTrack*>(GetTrack(vID));
	EMMediaTrack* opTrack = GetTrack(opTrans -> GetParentID());

	opTrack -> SetDestination(opTrans -> GetDestination());
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
	return MediaCommandDeleteTrack::ExecuteE(p_opParameterOne, NULL, NULL);
}

EMMediaTrack* MediaCommandDeleteVideoTransitionTrack::GetTrack(uint32 p_vID)
{
	EMMediaTrack* opTrack; 
	EMMediaTrackRepository* opUsed = EMMediaEngine::Instance() -> GetMediaProject() -> GetUsedTracks();
	EMMediaTrackRepository* opFree = EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks();

	opUsed -> LockContainer();
	opTrack = opUsed -> Find(p_vID);
	opUsed -> UnlockContainer();
	if(opTrack == NULL)
	{
		opFree -> LockContainer();
		opTrack = opFree -> Find(p_vID);
		opFree -> UnlockContainer();
	}

	if(opTrack == NULL)
	{
		EMDebugger("MediaCommandCreateVideoTransitionTrack - Couldn't find track!!");
	}

	return opTrack;
}