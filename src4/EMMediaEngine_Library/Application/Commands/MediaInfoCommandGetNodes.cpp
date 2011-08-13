

#include "MediaInfoCommandGetNodes.h"

#include "EMMediaEngine.h"
#include "EMMediaTrack.h"
#include "EMMediaProject.h"
#include "EMMediaTrackRepository.h"

MediaInfoCommandGetNodes::MediaInfoCommandGetNodes()
{
}

void* MediaInfoCommandGetNodes::ExecuteE(void* p_upTrackID, void* p_upParamID, void* p_upParameterThree)
{
	EMMediaTrack* opRecentTrack = NULL;

	int32 vTrackID = -1;

	if(p_upTrackID != NULL)
		vTrackID = *(static_cast<int32*>(p_upTrackID));

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
		EMDebugger("ERROR! Can't refere to the trackID passed to MediaInfoCommandGetNodes");
		return NULL;
	}

	return static_cast<void*>(opRecentTrack -> GetRubberBandNodeObject(vParamID));
 
}

bool MediaInfoCommandGetNodes::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetNodes::IsUndoable()
{
	return false;
}

void MediaInfoCommandGetNodes::UndoE()
{

}
