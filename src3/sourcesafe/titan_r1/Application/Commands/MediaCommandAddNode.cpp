

#include "MediaCommandAddNode.h"

#include "EMMediaEngine.h"
#include "EMMediaTrack.h"
#include "EMMediaProject.h"
#include "EMMediaTrackRepository.h"

MediaCommandAddNode::MediaCommandAddNode()
{
}

void* MediaCommandAddNode::ExecuteE(void* p_upValues, void* p_upTrackID, void* p_upParamID)
{
	EMMediaTrack* opRecentTrack = NULL;

	int32 vTrackID = -1;

	if(p_upTrackID != NULL)
		vTrackID = *(static_cast<int32*>(p_upTrackID));

	int64* vpValues = (static_cast<int64*>(p_upValues));

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
		EMDebugger("ERROR! Can't refere to the trackID passed to MediaCommandAddNode");
		return NULL;
	}

	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);

	return static_cast<void*>(opRecentTrack -> AddRubberBandNode(vpValues, vParamID));
 
}

bool MediaCommandAddNode::RequiresParameters()
{
	return true;
}

bool MediaCommandAddNode::IsUndoable()
{
	return false;
}

void MediaCommandAddNode::UndoE()
{

}
