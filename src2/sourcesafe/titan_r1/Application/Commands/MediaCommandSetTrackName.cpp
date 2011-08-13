#include "MediaCommandSetTrackName.h"

#include "EMMediaTrack.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaTrackRepository.h"
#include "EMMediaTransitionTrackRepository.h"

MediaCommandSetTrackName::MediaCommandSetTrackName(EMMediaTrack* p_opRecentTrack, string p_oRecentName)
	:	m_opRecentTrack(NULL),
		m_oRecentName(p_oRecentName)
{
}

void* MediaCommandSetTrackName::ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree)
{
	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();

	int32 vID = *(static_cast<int32*>(p_opParameterOne));
	string oName(static_cast<const char*>(p_opParameterTwo));

	EMMediaTrack* opTrack = NULL;
	opProject -> GetUsedTracks() -> LockContainer();
	try
	{
		opTrack = opProject -> GetUsedTracks() -> Find(vID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaCommandSetTrackName::ExecuteE");
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
			EMDebugger("ERROR! Exception in MediaCommandSetTrackName::ExecuteE");
		}
		opProject -> GetUnusedTracks() -> UnlockContainer();
	}

	opProject -> GetVideoTransitionTrackRepository() -> UnlockContainer();
	if(opTrack == NULL)
	{
		opProject -> GetVideoTransitionTrackRepository() -> LockContainer();
		try
		{
			opTrack = opProject -> GetVideoTransitionTrackRepository() -> Find(vID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandSetTrackName::ExecuteE");
		}
		opProject -> GetVideoTransitionTrackRepository() -> UnlockContainer();
	}

	if(opTrack == NULL)
		return NULL; //TODO: Throw exception instead?

	m_opRecentTrack = opTrack;
	m_oRecentName = opTrack -> GetName();

	opTrack -> SetName(oName);
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);

	return NULL;
}

bool MediaCommandSetTrackName::RequiresParameters()
{
	return true;
}

void MediaCommandSetTrackName::UndoE()
{
	//TODO: Fix so we don't use a direct pointer here! Fucks up system if track has been deleted!!!!!!!!
	if(m_opRecentTrack != NULL)
		m_opRecentTrack -> SetName(m_oRecentName);
}

bool MediaCommandSetTrackName::IsUndoable()
{
	return false;
}

EMCommand* MediaCommandSetTrackName::CloneAndClear()
{
	return EM_new MediaCommandSetTrackName(m_opRecentTrack, m_oRecentName);
}
