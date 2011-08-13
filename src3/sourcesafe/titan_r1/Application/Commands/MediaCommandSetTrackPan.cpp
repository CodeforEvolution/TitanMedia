#include "MediaCommandSetTrackPan.h"

#include "EMMediaEngine.h"
#include "EMCommandRepository.h"
#include "CommandIDs.h"
#include "EMMediaProject.h"
#include "EMMediaTrack.h"
#include "EMMediaTimer.h"
#include "EMMediaTrackRepository.h"
#include "EMMediaUtility.h"

MediaCommandSetTrackPan::MediaCommandSetTrackPan(EMMediaTrack* p_opRecentTrack, int64 p_vRecentPan)
	:	m_opRecentTrack(p_opRecentTrack),
		m_vRecentPan(p_vRecentPan)
{
}

void* MediaCommandSetTrackPan::ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree)
{
	//;//cout_commented_out_4_release << "MediaCommandSetTrackPan" << endl;
	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();

	int32* vID = static_cast<int32*>(p_opParameterOne);
	int32 vPan = *(static_cast<int32*>(p_opParameterTwo));

	EMMediaTrack* opTrack = NULL;
	opProject -> GetUsedTracks() -> LockContainer();
	try
	{
		opTrack = opProject -> GetUsedTracks() -> Find(*vID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaCommandSetTrackPan::ExecuteE");
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
			EMDebugger("ERROR! Exception in MediaCommandSetTrackPan::ExecuteE");
		}
		opProject -> GetUnusedTracks() -> UnlockContainer();
	}
		
//	delete vID;
	
	if(opTrack == NULL)
		return NULL; //TODO: Throw exception instead?
	
	m_opRecentTrack = opTrack;
	m_vRecentPan = opTrack -> GetPan();
	opTrack -> SetPan(vPan);
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
	EMMediaTimer::Instance() -> SeekToFrame(EMMediaTimer::Instance() -> NowFrame());
	return NULL;
}

bool MediaCommandSetTrackPan::RequiresParameters()
{
	return true;
}

void MediaCommandSetTrackPan::UndoE()
{
	if(m_opRecentTrack != NULL)
	{
		int32 vTrackID = m_opRecentTrack -> GetID();
		int32 vPanValue = m_vRecentPan;
		EMMediaEngine::Instance() ->GetCommandRepository() ->ExecuteCommand(MEDIA_COMMAND_SET_TRACK_PAN, &vTrackID, &vPanValue);
	}
}

bool MediaCommandSetTrackPan::IsUndoable()
{
	return false;
}

EMCommand* MediaCommandSetTrackPan::CloneAndClear()
{
	return EM_new MediaCommandSetTrackPan(m_opRecentTrack, m_vRecentPan);
}

