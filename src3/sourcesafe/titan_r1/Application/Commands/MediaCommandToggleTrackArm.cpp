#include "MediaCommandToggleTrackArm.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaTrack.h"
#include "EMMediaTimer.h"
#include "EMMediaTrackRepository.h"

MediaCommandToggleTrackArm::MediaCommandToggleTrackArm(EMMediaTrack* p_opRecentTrack)
	:	m_opRecentTrack(p_opRecentTrack)
{
}

void* MediaCommandToggleTrackArm::ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree)
{
	//;//cout_commented_out_4_release << "MediaCommandToggleTrackArm" << endl;
	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();
	int32* vID = static_cast<int32*>(p_opParameterOne);
	
	EMMediaTrack* opTrack = NULL;
	opProject -> GetUsedTracks() -> LockContainer();
	try
	{
		opTrack = opProject -> GetUsedTracks() -> Find(*vID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaCommandToggleTrackArm::ExecuteE");
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
			EMDebugger("ERROR! Exception in MediaCommandToggleTrackArm::ExecuteE");
		}
		opProject -> GetUnusedTracks() -> UnlockContainer();
	}
	if(opTrack == NULL)
	{
		EMDebugger("ERROR! Couldn't find the MediaTrack!");
		return NULL; //TODO: Throw exception instead?
	}
	
	m_opRecentTrack = opTrack;
	opTrack -> SetArmed(! opTrack -> IsArmed());
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);

	;//cout_commented_out_4_release << "Setting track " << opTrack -> GetID() << " to have armstatus: " << (opTrack -> IsArmed() ? "ON" : "OFF") << endl;

//	EMMediaTimer* opTimer = EMMediaTimer::Instance();
//	opTimer -> SeekToFrame(opTimer -> NowFrame());

	return NULL;
}

bool MediaCommandToggleTrackArm::RequiresParameters()
{
	return true;
}

void MediaCommandToggleTrackArm::UndoE()
{
	//TODO: Fix so we don't use a direct pointer here! Fucks up system if track has been deleted!
	if(m_opRecentTrack != NULL)
		m_opRecentTrack -> SetArmed(! m_opRecentTrack -> IsArmed());
}

bool MediaCommandToggleTrackArm::IsUndoable()
{
	return false;
}

EMCommand* MediaCommandToggleTrackArm::CloneAndClear()
{
	return EM_new MediaCommandToggleTrackArm(m_opRecentTrack);
}

