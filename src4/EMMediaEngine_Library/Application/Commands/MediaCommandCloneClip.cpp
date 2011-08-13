#include "MediaCommandCloneClip.h"

#include "EMMediaEngine.h"
#include "EMCommandRepository.h"
#include "CommandIDs.h"
#include "EMMediaProject.h"
#include "EMMediaTrack.h"
#include "EMMediaClipRepository.h"
#include "EMMediaClip.h"
#include "EMMediaTimer.h"
#include "EMMediaFormat.h"
#include "EMMediaEffectTrack.h"
#include "EMMediaEffectTrackRepository.h"
#include "EMMediaTrackRepository.h"
#include "EMMediaTransitionTrackRepository.h"
#include "EMMediaUtility.h"

//Used for command instantiation
MediaCommandCloneClip::MediaCommandCloneClip()
	:	m_vClipCopyID(-1)
{
} 

//Constructor used for Undo-cloning
MediaCommandCloneClip::MediaCommandCloneClip(int32 p_vClipCopyID)
	:	m_vClipCopyID(p_vClipCopyID)
{
}

void* MediaCommandCloneClip::ExecuteE(void* p_upClipID, void* p_upNewStart, void* p_upTrackID)
{
	EMMediaClip* opClip = NULL;
	EMMediaTrack* opRecentTrack = NULL;

	int32 vClipID = *(static_cast<int32*>(p_upClipID));
	int64 vNewStart = *(static_cast<int64*>(p_upNewStart));
	int32 vTrackID = -1;
	bool vTrackWasEmpty = false;
	if(p_upTrackID != NULL)
		vTrackID = *(static_cast<int32*>(p_upTrackID));
	else
		EMDebugger("ERROR! Track own is NULL in clone clip!");

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
		vTrackWasEmpty = true;
	}

	
	if(opRecentTrack == NULL)
	{
		opProject -> GetVideoTransitionTrackRepository() -> LockContainer();
		try
		{
			opRecentTrack = opProject -> GetVideoTransitionTrackRepository() -> Find(vTrackID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Caught an exception in MediaCommandCloneClip::ExecuteE (2)");
		}
		opProject -> GetVideoTransitionTrackRepository() -> UnlockContainer();
		vTrackWasEmpty = false;
	}

	EMMediaClipRepository* opAudioClipRepository = opProject -> GetAudioClipRepository();
	EMMediaClipRepository* opVideoClipRepository = opProject -> GetVideoClipRepository();
	EMMediaClipRepository* opMIDIClipRepository = opProject -> GetMIDIClipRepository();
	EMMediaClipRepository* opVideoTransitionClipRepository = opProject -> GetVideoTransitionClipRepository();
	EMMediaClipRepository* opRepository = NULL;
	EMMediaEffectTrackRepository* opFXTracks = EMMediaEffectTrackRepository::Instance();

	opAudioClipRepository -> LockContainer();
	try
	{
		opClip = opAudioClipRepository -> Find(vClipID);
		if(opClip != NULL)
			opRepository = opAudioClipRepository;
	}
	catch(...)
	{
		EMDebugger("ERROR! Caught an exception in MediaCommandCloneClip::ExecuteE (3)");
	}
	opAudioClipRepository -> UnlockContainer();

	if(opClip == NULL)
	{
		opVideoClipRepository -> LockContainer();
		try
		{
			opClip = opVideoClipRepository -> Find(vClipID);
			if(opClip != NULL)
				opRepository = opVideoClipRepository;
		}
		catch(...)
		{
			EMDebugger("ERROR! Caught an exception in MediaCommandCloneClip::ExecuteE (4)");
		}
		opVideoClipRepository -> UnlockContainer();
	} 
	
	if(opClip == NULL)
	{
		opMIDIClipRepository -> LockContainer();
		try
		{
			opClip = opMIDIClipRepository -> Find(vClipID);
			if(opClip != NULL)
				opRepository = opMIDIClipRepository;
		}
		catch(...)
		{
			EMDebugger("ERROR! Caught an exception in MediaCommandCloneClip::ExecuteE (4)");
		}
		opMIDIClipRepository -> UnlockContainer();
	}

	if(opClip == NULL)
	{
		opVideoTransitionClipRepository -> LockContainer();
		try
		{
			opClip = opVideoTransitionClipRepository -> Find(vClipID);
			if(opClip != NULL)
				opRepository = opVideoTransitionClipRepository;
		}
		catch(...)
		{
			EMDebugger("ERROR! Caught an exception in MediaCommandCloneClip::ExecuteE (4)");
		}
		opVideoTransitionClipRepository -> UnlockContainer();
	}

	if(opClip == NULL)
		EMDebugger("ERROR! Could not find clip in any clip repository!");

	EMMediaClip* opClipClone = opClip -> CloneClip(EMMediaUtility::Instance() -> TimeToFrames(vNewStart, EMMediaUtility::Instance() -> GetSystemAudioFormat()), opRecentTrack);
	opClipClone -> InitCheckE();
//	vOldStart = m_opClip -> GetStart();
	opClipClone -> SetTrack(opRecentTrack);

	opRepository -> LockContainer();
	try
	{
		opRepository -> Add(opClipClone);
	}
	catch(...)
	{
		EMDebugger("ERROR! Caught an exception in MediaCommandCloneClip::ExecuteE (5)");
	}
	opRepository -> UnlockContainer();
	

	if(opClip -> HasEffectTrack())
	{
		EMMediaEffectTrack* opFXTrack = opFXTracks -> Find(opClip -> GetEffectTrackID());
		EMMediaEffectTrack* opFXClone = EM_new EMMediaEffectTrack(*opFXTrack);

		eo << "Cloned FX-track, new id: " << opFXClone ->GetID() << ef;
		opFXTracks -> Add(opFXClone);

		opFXClone -> SetOutput(opClipClone -> GetDestination());
		opClipClone -> SetDestination(opFXClone);
		opClipClone -> SetEffectTrackID(opFXClone -> GetID());
	}

	EMMediaTimer* opTimer = EMMediaTimer::Instance();
	if(vTrackWasEmpty)
	{
		opProject -> GetUnusedTracks() -> RemoveTrack(opRecentTrack);
		opProject -> GetUsedTracks() -> AddTrack(opRecentTrack);
	}

	m_vReturnValue = opClipClone -> GetID();
	m_vClipCopyID = m_vReturnValue;

	EMMediaTimerState eMode = opTimer -> GetState();
	if(eMode == EM_PLAYING || eMode == EM_RECORDING)
		opTimer -> SetState(EM_STOPPED);
	opTimer -> SeekToFrame(opTimer -> NowFrame());
	if(eMode == EM_PLAYING || eMode == EM_RECORDING)
		opTimer -> SetState((eMode == EM_RECORDING ? EM_COUNTING_IN_FOR_RECORDING : EM_COUNTING_IN_FOR_PLAYBACK));

	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
	
	return static_cast<void*>(&m_vReturnValue);
}

bool MediaCommandCloneClip::RequiresParameters()
{
	return true;
}

bool MediaCommandCloneClip::IsUndoable()
{
	return false;
}

EMCommand* MediaCommandCloneClip::CloneAndClear()
{
	return EM_new MediaCommandCloneClip(m_vClipCopyID);
}

void MediaCommandCloneClip::UndoE()
{
	if(m_vClipCopyID != -1)
		EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(MEDIA_COMMAND_DELETE_CLIP, &m_vClipCopyID, NULL, NULL);
}
