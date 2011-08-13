#include "MediaCommandSetTrackFader.h"

#include "EMMediaEngine.h"
#include "EMCommandRepository.h"
#include "CommandIDs.h"
#include "EMMediaProject.h"
#include "EMMediaTrack.h"
#include "EMMediaTimer.h"
#include "EMMediaTrackRepository.h"
#include "EMMediaUtility.h"

MediaCommandSetTrackFader::MediaCommandSetTrackFader(EMMediaTrack* p_opRecentTrack, int64 p_vRecentFader)
	:	m_opRecentTrack(p_opRecentTrack),
		m_vRecentFader(p_vRecentFader)
{
}

void* MediaCommandSetTrackFader::ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree)
{
	//;//cout_commented_out_4_release << "MediaCommandSetTrackFader" << endl;
	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();
	EMMediaTimer* opTimer = EMMediaTimer::Instance();

	int32* vID = static_cast<int32*>(p_opParameterOne);
	int32 vFader = *(static_cast<int32*>(p_opParameterTwo));

	EMMediaTrack* opTrack = NULL;
	opProject -> GetUsedTracks() -> LockContainer();
	try
	{
		opTrack = opProject -> GetUsedTracks() -> Find(*vID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaCommandSetTrackFader::ExecuteE");
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
			EMDebugger("ERROR! Exception in MediaCommandSetTrackFader::ExecuteE");
		}
		opProject -> GetUnusedTracks() -> UnlockContainer();
	}
	
	if(opTrack == NULL)
		EMDebugger("ERROR! Bad track ID in MediaCommandSetTrackFader");

	m_opRecentTrack = opTrack;
	m_vRecentFader = opTrack -> GetFader();

	opTrack -> SetFader(vFader);
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
	
	EMMediaTimerState eMode = opTimer -> GetState();
	if(eMode == EM_PLAYING || eMode == EM_RECORDING)
		opTimer -> SetState(EM_STOPPED);
	opTimer -> SeekToFrame(opTimer -> NowFrame());
	if(eMode == EM_PLAYING || eMode == EM_RECORDING)
		opTimer -> SetState((eMode == EM_RECORDING ? EM_COUNTING_IN_FOR_RECORDING : EM_COUNTING_IN_FOR_PLAYBACK));
	return NULL;
}

bool MediaCommandSetTrackFader::RequiresParameters()
{
	return true;
}

void MediaCommandSetTrackFader::UndoE()
{
	if(m_opRecentTrack != NULL)
	{
		int32 vTrackID = m_opRecentTrack ->GetID();
		int32 vFaderValue = m_vRecentFader;
		EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(MEDIA_COMMAND_SET_TRACK_FADER, &vTrackID, &vFaderValue);
	}
}

bool MediaCommandSetTrackFader::IsUndoable()
{
	return false;
}

EMCommand* MediaCommandSetTrackFader::CloneAndClear()
{
	return EM_new MediaCommandSetTrackFader(m_opRecentTrack, m_vRecentFader);
}

