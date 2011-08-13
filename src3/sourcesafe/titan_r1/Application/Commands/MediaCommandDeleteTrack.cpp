#include "MediaCommandDeleteTrack.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaTrackRepository.h"
#include "EMMediaTimer.h"
#include "EMBeMediaUtility.h"
#include "EMMediaVideoTransitionTrack.h"
#include "EMMediaTransitionTrackRepository.h"
#include "EMMediaEffectTrack.h"

MediaCommandDeleteTrack::MediaCommandDeleteTrack()
	:	m_vDeletedMediaTrackID(-1),
		m_vFxID(-1)
{
}

MediaCommandDeleteTrack::MediaCommandDeleteTrack(int32 p_vMediaTrackID, int32 p_vFxID)
	:	m_vDeletedMediaTrackID(p_vMediaTrackID),
		m_vFxID(p_vFxID)
{
}

void* MediaCommandDeleteTrack::ExecuteE(void* p_upMediaTrackID, void*, void*)
{
	int32 vTrackID = *static_cast<int32*>(p_upMediaTrackID);
	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();
	EMMediaTimer* opTimer = EMMediaTimer::Instance();

	bool vTrackWasUsed = false;
	bool vTrackWasTransition = false;
//	EMMediaTrackRepository* opRepository = opProject -> GetUnusedTracks();
	
	EMMediaTrack* opTrack = NULL;
	opProject -> GetUsedTracks() -> LockContainer();
	try
	{
		opTrack = opProject -> GetUsedTracks() -> Find(vTrackID);
		vTrackWasUsed = true;
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaCommandDeleteTrack::ExecuteE");
	}
	opProject -> GetUsedTracks() -> UnlockContainer();

	if(opTrack == NULL)
	{
		opProject -> GetUnusedTracks() -> LockContainer();
		try
		{
			opTrack = opProject -> GetUnusedTracks() -> Find(vTrackID);
			vTrackWasUsed = false;
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandDeleteTrack::ExecuteE");
		}
		opProject -> GetUnusedTracks() -> UnlockContainer();
	}

	if(opTrack == NULL)
	{
		opProject -> GetVideoTransitionTrackRepository() -> LockContainer();
		try
		{
			opTrack = dynamic_cast<EMMediaTrack*>(opProject -> GetVideoTransitionTrackRepository() -> Find(vTrackID));
			vTrackWasTransition = true;
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandDeleteTrack::ExecuteE");
		}
		opProject -> GetVideoTransitionTrackRepository() -> UnlockContainer();
	}

	if(opTrack == NULL)
	{
		EMDebugger("ERROR! Couldn't find the track's media id!");
		return NULL;
	}

	try
	{
		if(opProject -> LockAudio())
		{
			if(opProject -> LockVideo())
			{
				if(opTrack -> GetTrackDestination() != NULL && opTrack -> GetTrackDestination() -> GetPosition() == EM_EFFECT)
				{
					m_vFxID = opTrack -> GetTrackDestination() -> GetID();
					dynamic_cast<EMMediaEffectTrack*>(opTrack -> GetTrackDestination()) -> SetDeleted(true);
				}

				opTrack -> SetDeleted(true);
				EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
				m_vDeletedMediaTrackID = opTrack -> GetID();

				if(vTrackWasUsed)
				{
//					opProject -> GetUsedTracks() -> LockContainer();
					opProject -> GetUsedTracks() -> RemoveTrack(opTrack);
//					opProject -> GetUsedTracks() -> UnlockContainer();

//					opProject -> GetUnusedTracks() -> LockContainer();
					opProject -> GetUnusedTracks() -> AddTrack(opTrack);
//					opProject -> GetUnusedTracks() -> UnlockContainer();
				}

				if(opTrack -> GetType() == EM_TYPE_TRANSITION)
					static_cast<EMMediaVideoTransitionTrack*>(opTrack) -> Disconnect();

				opProject -> UnlockVideo();
			}
			else
				EMDebugger("ERROR! MediaCommandDeleteTrack could not acquire video processing lock!");
			
			opProject -> UnlockAudio();
		}
		else
			EMDebugger("ERROR! MediaCommandDeleteTrack could not acquire audio processing lock!");
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaCommandDeleteTrack::ExecuteE");
	}
	
	EMMediaTimerState eMode = opTimer -> GetState();
	if(eMode == EM_PLAYING || eMode == EM_RECORDING)
		opTimer -> SetState(EM_STOPPED);
	opTimer -> SeekToFrame(opTimer -> NowFrame());
	if(eMode == EM_PLAYING || eMode == EM_RECORDING)
		opTimer -> SetState((eMode == EM_RECORDING ? EM_COUNTING_IN_FOR_RECORDING : EM_COUNTING_IN_FOR_PLAYBACK));
	
	return NULL;
}

bool MediaCommandDeleteTrack::RequiresParameters()
{
	return true;
}

bool MediaCommandDeleteTrack::IsUndoable()
{
	return false;
}

void MediaCommandDeleteTrack::UndoE()
{
	int32 vTrackID = m_vDeletedMediaTrackID;
	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();
//	EMMediaTrackRepository* opRepository = opProject -> GetUnusedTracks();
	
	EMMediaTrack* opTrack = NULL;
	opProject -> GetUsedTracks() -> LockContainer();
	try
	{
		opTrack = opProject -> GetUsedTracks() -> Find(vTrackID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaCommandDeleteTrack::ExecuteE");
	}
	opProject -> GetUsedTracks() -> UnlockContainer();

	if(opTrack == NULL)
	{
		opProject -> GetUnusedTracks() -> LockContainer();
		try
		{
			opTrack = opProject -> GetUnusedTracks() -> Find(vTrackID);
//			vTrackWasUsed = false;
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandDeleteTrack::ExecuteE");
		}
		opProject -> GetUnusedTracks() -> UnlockContainer();
	}

	if(opTrack == NULL)
	{
		opProject -> GetVideoTransitionTrackRepository() -> LockContainer();
		try
		{
			opTrack = dynamic_cast<EMMediaTrack*>(opProject -> GetVideoTransitionTrackRepository() -> Find(vTrackID));
//			vTrackWasTransition = true;
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandDeleteTrack::ExecuteE");
		}
		opProject -> GetVideoTransitionTrackRepository() -> UnlockContainer();
	}

//	else
//	{
//		opRepository = opProject -> GetUsedTracks();
//		vTrackWasUsed = true;
//	}
	
	if(opTrack == NULL)
		EMDebugger("ERROR! Couldn't find the track object using that media track ID, inside MediaCommandDeleteTrack::UndoE!");

	try
	{

		if(opProject -> LockAudio())
		{
			if(opProject -> LockVideo())
			{
				if(m_vFxID != -1)
					static_cast<EMMediaEffectTrack*>(opTrack -> GetTrackDestination()) -> SetDeleted(false);

				opTrack -> SetDeleted(false);
				m_vDeletedMediaTrackID = opTrack -> GetID();

				if(opTrack -> GetType() == EM_TYPE_TRANSITION)
					static_cast<EMMediaVideoTransitionTrack*>(opTrack) -> Connect();

				opProject -> UnlockVideo();
			}
			else
				EMDebugger("ERROR! MediaCommandDeleteTrack could not acquire video processing lock!");
			
			opProject -> UnlockAudio();
		}
		else
			EMDebugger("ERROR! MediaCommandDeleteTrack could not acquire audio processing lock!");
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaCommandDeleteTrack::UndoE");
	}
	
	EMMediaTimer::Instance() -> SeekToFrame(EMMediaTimer::Instance() -> NowFrame());
}

EMCommand* MediaCommandDeleteTrack::CloneAndClear()
{
	return EM_new MediaCommandDeleteTrack(m_vDeletedMediaTrackID, m_vFxID);
}
