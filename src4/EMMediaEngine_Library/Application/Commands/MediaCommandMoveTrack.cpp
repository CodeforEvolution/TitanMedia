#include "MediaCommandMoveTrack.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaTrack.h"
#include "EMMediaTrackRepository.h"

MediaCommandMoveTrack::MediaCommandMoveTrack()
{
}

void* MediaCommandMoveTrack::ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree)
{
	//;//cout_commented_out_4_release << "MediaCommandMoveTrack" << endl;
	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();

	int32 vID = *(static_cast<int32*>(p_opParameterOne));
	uint32 vPrio = *(static_cast<uint32*>(p_opParameterTwo));

	EMMediaTrack* opTrack = NULL;
	opProject -> GetUsedTracks() -> LockContainer();
	try
	{
		opTrack = opProject -> GetUsedTracks() -> Find(vID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaCommandMoveTrack::ExecuteE");
	}
	opProject -> GetUsedTracks() -> UnlockContainer();
	if(opTrack == NULL)
	{
		opProject -> GetUnusedTracks() -> LockContainer();
		try
		{
			opTrack = opProject -> GetUnusedTracks() -> Find(vID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandMoveTrack::ExecuteE");
		}
		opProject -> GetUnusedTracks() -> UnlockContainer();
	}
	
	if(opTrack == NULL)
		return NULL; //TODO: Throw exception instead?

	m_opRecentTrack = opTrack;
	opTrack -> SetPriority(vPrio);
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
	return NULL;
}

bool MediaCommandMoveTrack::RequiresParameters()
{
	return true;
}

void MediaCommandMoveTrack::UndoE()
{
}

bool MediaCommandMoveTrack::IsUndoable()
{
	return false;
}
