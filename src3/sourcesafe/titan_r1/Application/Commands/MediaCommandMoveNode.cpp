

#include "MediaCommandMoveNode.h"

#include "EMMediaEngine.h"
#include "EMMediaTrack.h"
#include "EMMediaProject.h"
#include "EMMediaTrackRepository.h"
#include "EMRubberBandNode.h"

MediaCommandMoveNode::MediaCommandMoveNode()
{
}

void* MediaCommandMoveNode::ExecuteE(void* p_upEMNodePtr, void* p_upValues, void* p_upTrackID)
{
	EMMediaTrack* opRecentTrack = NULL;

	int32 vTrackID = -1;

	if(p_upTrackID != NULL)
		vTrackID = *(static_cast<int32*>(p_upTrackID));

	int64* vpValues = (static_cast<int64*>(p_upValues));

	EMRubberBandNode* opRubberBandNode = (static_cast<EMRubberBandNode*>(p_upEMNodePtr));


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
		EMDebugger("ERROR! Can't refere to the trackID passed to MediaCommandMoveNode");
		return NULL;
	}

	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
	opRecentTrack -> MoveRubberBandNode(opRubberBandNode, vpValues);

	return NULL;
 
}

bool MediaCommandMoveNode::RequiresParameters()
{
	return true;
}

bool MediaCommandMoveNode::IsUndoable()
{
	return false;
}

void MediaCommandMoveNode::UndoE()
{

}
