#include "MediaCommandToggleTrackSolo.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaTrack.h"
#include "EMMediaTimer.h"
#include "EMMediaTrackRepository.h"
#include "EMMediaUtility.h"

#include "MediaCommandSetActiveMIDITrack.h"

MediaCommandToggleTrackSolo::MediaCommandToggleTrackSolo(EMMediaTrack* p_opRecentTrack)
	:	m_opRecentTrack(p_opRecentTrack)
{
}

void* MediaCommandToggleTrackSolo::ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree)
{
	EMMediaEngine* opEngine = EMMediaEngine::Instance(); 
	EMMediaProject* opProject = opEngine -> GetMediaProject();
	int32 vID = *(static_cast<int32*>(p_opParameterOne));
	
	EMMediaTrack* opTrack = NULL;
	opProject -> GetUsedTracks() -> LockContainer();
	try
	{
		opTrack = opProject -> GetUsedTracks() -> Find(vID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaCommandToggleTrackSolo::ExecuteE");
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
			EMDebugger("ERROR! Exception in MediaCommandToggleTrackSolo::ExecuteE");
		}
		opProject -> GetUnusedTracks() -> UnlockContainer();
	}

	if(opTrack == NULL)
		return NULL;
	
	m_opRecentTrack = opTrack;
	opTrack -> SetSoloed(! opTrack -> IsSoloed());
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
	EMMediaTimer::Instance() -> SeekToFrame(EMMediaTimer::Instance() -> NowFrame());
	return NULL;
}

bool MediaCommandToggleTrackSolo::RequiresParameters()
{
	return true;
}

void MediaCommandToggleTrackSolo::UndoE()
{
	//TODO: Fix so we don't use a pointer here! Fucks up system if track has been deleted!
	if(m_opRecentTrack != NULL)
		m_opRecentTrack -> SetSoloed(! m_opRecentTrack -> IsSoloed());
}

bool MediaCommandToggleTrackSolo::IsUndoable()
{
	return false;
}

EMCommand* MediaCommandToggleTrackSolo::CloneAndClear()
{
	return EM_new MediaCommandToggleTrackSolo(m_opRecentTrack);
}

