

#include "MediaCommandDeleteNode.h"

#include "EMMediaEngine.h"
#include "EMMediaTrack.h"
#include "EMMediaProject.h"
#include "EMMediaTrackRepository.h"
#include "EMRubberBandNode.h"

MediaCommandDeleteNode::MediaCommandDeleteNode()
{
}

void* MediaCommandDeleteNode::ExecuteE(void* p_upEMNodePtr, void* p_upTrackID, void* p_upParamID)
{
	EMMediaTrack* opRecentTrack = NULL;

	int32 vTrackID = -1;

	if(p_upTrackID != NULL)
		vTrackID = *(static_cast<int32*>(p_upTrackID));

	EMRubberBandNode* opRubberBandNode = (static_cast<EMRubberBandNode*>(p_upEMNodePtr));

	int vParamID = *(static_cast<int*>(p_upParamID));


	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();
	opProject -> GetUsedTracks() -> LockContainer();
	try
	{
		opRecentTrack = opProject -> GetUsedTracks() -> Find(vTrackID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Caught an exception in MediaCommandCloneClip::ExecuteE (1)");
	}
	opProject -> GetUsedTracks() -> UnlockContainer();

	if(opRecentTrack == NULL)
	{
		opProject -> GetUnusedTracks() -> LockContainer();
		try
		{
			opRecentTrack = opProject -> GetUnusedTracks() -> Find(vTrackID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Caught an exception in MediaCommandCloneClip::ExecuteE (2)");
		}
		opProject -> GetUnusedTracks() -> UnlockContainer();
	}

	if(opRecentTrack == NULL)
	{
		EMDebugger("ERROR! Can't refere to the trackID passed to MediaCommandDeleteNode");
		return NULL;
	}

	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);

	opRecentTrack -> DeleteRubberBandNode(opRubberBandNode, vParamID);

	return NULL;
 
}

bool MediaCommandDeleteNode::RequiresParameters()
{
	return true;
}

bool MediaCommandDeleteNode::IsUndoable()
{
	return false;
}

void MediaCommandDeleteNode::UndoE()
{

}
