#include "MediaCommandCreateTrack.h"
#include "EMMediaTrack.h"
#include "EMMediaEngine.h"
#include "EMMediaTimer.h"
#include "EMMediaProject.h"
#include "EMMediaTrackRepository.h"
#include "EMCommandRepository.h"
#include "CommandIDs.h"
#include "EMMediaEffectTrackRepository.h"
#include "EMMediaEffectTrack.h"

MediaCommandCreateTrack::MediaCommandCreateTrack()
{
}

MediaCommandCreateTrack::~MediaCommandCreateTrack()
{
}

void MediaCommandCreateTrack::ExecuteCreateTrack(EMMediaType p_eType, const char* p_vpName, uint32 p_vPrio)
{
//	_ASSERT(p_vpName != NULL);
	//;//cout_commented_out_4_release << "MediaCommandCreateTrack" << endl;
	EMMediaTrack* opNewTrack = NULL;
	try
	{
		opNewTrack = EMMediaTrack::CreateEMTrack(p_eType, string(p_vpName), p_vPrio);
		/*opNewTrack -> InitCheckE();
		(EMMediaEngine::Instance() -> GetMediaProject()) -> GetUnusedTracks() -> LockContainer();
		(EMMediaEngine::Instance() -> GetMediaProject()) -> GetUnusedTracks() -> Add(opNewTrack);
		(EMMediaEngine::Instance() -> GetMediaProject()) -> GetUnusedTracks() -> UnlockContainer(); */
	}
	catch(bad_alloc)
	{
		eo << "ERROR! Caught a bad_alloc in MediaCommandCreateTrack" << ef;
		//FatalExit(1); 
		//TODO
	}
	catch(...)
	{
		eo << "1 ERROR! Cought an exception in MediaCommandCreateTrack" << ef;
		//FatalExit(1);
	}
}

void* MediaCommandCreateTrack::ExecuteCreateTrackP(EMMediaType p_eType, const char* p_vpName, uint32 p_vPrio)
{
//	_ASSERT(p_vpName != NULL);
	EMMediaTrack* opNewTrack = NULL;
	try
	{
		EMMediaEngine* opEngine = EMMediaEngine::Instance();
		EMMediaProject* opProject = opEngine -> GetMediaProject();
		EMMediaTimer* opTimer = EMMediaTimer::Instance();

		if(opProject != NULL && opTimer != NULL)
		{
			EMMediaTimerState eMode = opTimer -> GetState();
			opNewTrack = EMMediaTrack::CreateEMTrack(p_eType, string(p_vpName), p_vPrio);
			if(((p_eType & EM_TYPE_ANY_VIDEO) > 0) && opProject->IsPlaying())
				opProject -> ShouldProduceVideo(true);
			if(eMode == EM_PLAYING || eMode == EM_RECORDING)
				opTimer -> SetState(EM_STOPPED);
			opNewTrack -> InitCheckE();
			opTimer -> SeekToFrame(opTimer -> NowFrame());
			opProject -> GetUnusedTracks() -> AddTrack(opNewTrack);
			EMMediaTrackRepository::m_opTracks -> push_back(opNewTrack);
			EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
			
			if(eMode == EM_PLAYING || eMode == EM_RECORDING)
				opTimer -> SetState((eMode == EM_RECORDING ? EM_COUNTING_IN_FOR_RECORDING : EM_COUNTING_IN_FOR_PLAYBACK));

			eo << "Created MediaTrack with ID " << opNewTrack -> GetID() << ef;
		}
		else
			eo << "ERROR! There is no media project in memory, so no tracks can be created!" << ef;
	}
	catch(...)
	{
		eo << "ERROR! Caught an exception in MediaCommandCreateTrack" << ef;
	}
	
/*
	if((opNewTrack -> GetType() & EM_TYPE_ANY_AUDIO) > 0)
	{
		char* vpFXTrackName = "Effect track";
		int32 vEffectTrackID = *static_cast<int32*>(EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(MEDIA_COMMAND_CREATE_EFFECT_TRACK, vpFXTrackName, NULL, NULL));
		EMMediaEffectTrack* opFXTrack = EMMediaEffectTrackRepository::Instance() -> Find(vEffectTrackID);

		opNewTrack -> SetTrackDestination(opFXTrack);
	} */

	m_vReturnValue = opNewTrack -> GetID();
	return static_cast<void*>(&m_vReturnValue);
}

bool MediaCommandCreateTrack::RequiresParameters()
{
//	MessageBox(NULL, "Now inside MediaCommandCreateTrack", "", MB_OK);
	return true;
}

bool MediaCommandCreateTrack::IsUndoable()
{
	return false;
}