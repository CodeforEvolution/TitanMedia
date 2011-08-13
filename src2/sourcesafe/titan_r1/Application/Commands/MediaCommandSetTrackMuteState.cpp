#include "MediaCommandSetTrackMuteState.h"

#include "EMMediaEngine.h"
#include "EMCommandRepository.h"
#include "CommandIDs.h"
#include "EMMediaProject.h"
#include "EMMediaTrack.h"
#include "EMMediaTimer.h"
#include "EMMediaTrackRepository.h"
#include "EMMediaTransitionTrackRepository.h"
#include "EMMediaUtility.h"

MediaCommandSetTrackMuteState::MediaCommandSetTrackMuteState(EMMediaTrack* p_opRecentTrack, EMMuteState p_eOldMuteState)
	:	m_opRecentTrack(p_opRecentTrack),
		m_eOldMuteState(p_eOldMuteState)
{
}

void* MediaCommandSetTrackMuteState::ExecuteE(void* p_upTrackID, void* p_upState, void*)
{
	//;//cout_commented_out_4_release << "MediaCommandSetTrackMuteState" << endl;
	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();

	int32* vID = static_cast<int32*>(p_upTrackID);
	EMMuteState eState = *static_cast<EMMuteState*>(p_upState);
	
	EMMediaTrack* opTrack = NULL;
	opProject -> GetUsedTracks() -> LockContainer();
	try
	{
		opTrack = opProject -> GetUsedTracks() -> Find(*vID); 
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaCommandSetTrackMuteState::ExecuteE");
	}
	opProject -> GetUsedTracks() -> UnlockContainer();
	if(opTrack == NULL)
	{
		opProject -> GetUnusedTracks() -> LockContainer();
		try
		{
			opTrack = opProject -> GetUnusedTracks() -> Find(*vID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandSetTrackMuteState::ExecuteE");
		}
		opProject -> GetUnusedTracks() -> UnlockContainer();
	}

	if(opTrack == NULL)
	{
		opProject -> GetVideoTransitionTrackRepository() -> LockContainer();
		try
		{
			opTrack = opProject -> GetVideoTransitionTrackRepository() -> Find(*vID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandSetTrackMuteState::ExecuteE");
		}
		opProject -> GetVideoTransitionTrackRepository() -> UnlockContainer();
	}

	if(opTrack == NULL)
		return NULL; //TODO: Throw exception instead?
	
	m_opRecentTrack = opTrack;

	m_eOldMuteState = opTrack -> GetMuteState();
	opTrack -> SetMuteState(eState);
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);

	EMMediaTimer::Instance() -> SeekToFrame(EMMediaTimer::Instance() -> NowFrame());
	return NULL;
}

bool MediaCommandSetTrackMuteState::RequiresParameters()
{
	return true;
}

void MediaCommandSetTrackMuteState::UndoE()
{
	if(m_opRecentTrack != NULL)
	{
		int32 vTrackID = m_opRecentTrack -> GetID();
		EMMediaEngine::Instance() -> GetCommandRepository()->ExecuteCommand(MEDIA_COMMAND_SET_TRACK_MUTE_STATE, &vTrackID, &m_eOldMuteState);
	}
}

bool MediaCommandSetTrackMuteState::IsUndoable()
{
	return false;
}

EMCommand* MediaCommandSetTrackMuteState::CloneAndClear()
{
	return EM_new MediaCommandSetTrackMuteState(m_opRecentTrack, m_eOldMuteState);
}

