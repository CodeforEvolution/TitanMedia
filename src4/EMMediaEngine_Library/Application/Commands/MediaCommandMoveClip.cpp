#include "MediaCommandMoveClip.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaTrack.h"
#include "EMMediaClipRepository.h"
#include "EMMediaClip.h"
#include "EMMediaTimer.h"
#include "EMMediaFormat.h"
#include "EMMediaTrackRepository.h"
#include "EMMediaUtility.h"
#include "EMMediaVideoTransitionClip.h"
#include "EMMediaMIDIEvent.h"
#include "EMMediaMIDIEventRepository.h"
#include "EMMIDIClip.h"

MediaCommandMoveClip::MediaCommandMoveClip(EMMediaClip* p_opClip, int64 p_vOldStart, EMMediaTrack* p_opRecentTrack)
	:	m_opClip(p_opClip),
		m_vOldStart(p_vOldStart),
		m_opRecentTrack(p_opRecentTrack)
{
}

void* MediaCommandMoveClip::ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree)
{
	//eo << "MediaCommandMoveClip" << ef;
	bool vTrackWasEmpty = false;
	int32 vClipID = *(static_cast<int32*>(p_opParameterOne));
	int64 vNewStart = *(static_cast<int64*>(p_opParameterTwo));
	int32 vTrackID = -1;
	if(p_opParameterThree != NULL)
		vTrackID = *(static_cast<int64*>(p_opParameterThree));

	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();
	EMMediaTimer* opTimer = EMMediaTimer::Instance();
 
	EMMediaTrack* opNewTrack = NULL;
	opProject -> GetUsedTracks() -> LockContainer();
	try
	{
		opNewTrack = opProject -> GetUsedTracks() -> Find(vTrackID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaCommandMoveClip::ExecuteE");
	}
	opProject -> GetUsedTracks() -> UnlockContainer();
	if(opNewTrack == NULL)
	{
		opProject -> GetUnusedTracks() -> LockContainer();
		try
		{
			opNewTrack = opProject -> GetUnusedTracks() -> Find(vTrackID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandMoveClip::ExecuteE");
		}
		opProject -> GetUnusedTracks() -> UnlockContainer();
		vTrackWasEmpty = true;
	}
	
	EMMediaClipRepository* opAudioClipRepository = opProject -> GetAudioClipRepository();
	EMMediaClipRepository* opVideoClipRepository = opProject -> GetVideoClipRepository();
	EMMediaClipRepository* opVideoTransitionClipRepository = opProject -> GetVideoTransitionClipRepository();
	EMMediaClipRepository* opMIDIClipRepository = opProject -> GetMIDIClipRepository();

	int64 vNewFrame = EMMediaUtility::Instance() -> TimeToFrames(vNewStart, EMMediaUtility::Instance() -> GetSystemAudioFormat());

	opAudioClipRepository -> LockContainer();
	try
	{
		m_opClip = opAudioClipRepository -> Find(vClipID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaCommandMoveClip::ExecuteE");
	}
	opAudioClipRepository -> UnlockContainer();
	if(m_opClip == NULL)
	{
		opVideoClipRepository -> LockContainer();
		try
		{
			m_opClip = opVideoClipRepository -> Find(vClipID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandMoveClip::ExecuteE");
		}
		opVideoClipRepository -> UnlockContainer();
	}
	if(m_opClip == NULL)
	{
		opVideoTransitionClipRepository -> LockContainer();
		try
		{
			m_opClip = opVideoTransitionClipRepository -> Find(vClipID);
			if(m_opClip != NULL)
			{
				EMVideoTransitionClip* opTrans = static_cast<EMVideoTransitionClip*>(m_opClip);
				opTrans -> MoveClip(vNewFrame);
			}
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandMoveClip::ExecuteE");
		}
		opVideoTransitionClipRepository -> UnlockContainer();
//		return NULL;
	}
	if(m_opClip == NULL)
	{
		opMIDIClipRepository -> LockContainer();
		try
		{
			m_opClip = opMIDIClipRepository -> Find(vClipID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandMoveClip::ExecuteE");
		}
		opMIDIClipRepository -> UnlockContainer();
	}


	if(m_opClip != NULL)
//		m_vOldStart = m_opClip -> GetFirstFrame(); // + m_opClip -> GetMarkInOffset();
		m_vOldStart = m_opClip -> GetStart();
	else
		return NULL; //TODO: Throw exception instead?

	//TODO: Make <track-move + time-change> atomic!!
//	bool vWasPlaying = opProject -> IsPlaying();
//	if(vWasPlaying)
//		opProject -> PausePlaybackE();

	EMMediaTimerState eMode = opTimer -> GetState();
	if(eMode == EM_PLAYING || eMode == EM_RECORDING)
		opTimer -> SetState(EM_STOPPED);

	m_opClip -> SetStart(vNewFrame);
	if((m_opClip -> GetType() & EM_TYPE_MIDI) > 0)
	{
		EMMIDIClip* opMIDIClip = static_cast<EMMIDIClip*>(m_opClip);

		list<EMMediaMIDIEvent*> oEventList;
		opMIDIClip -> GetAllMIDIEvents(&oEventList);
		list<EMMediaMIDIEvent*>::iterator oEventIterator;
		
		for(oEventIterator = oEventList.begin(); oEventIterator != oEventList.end(); oEventIterator++)
		{
			int64 vOffset = EMMediaUtility::Instance() -> FramesToTime(vNewFrame - m_vOldStart, EMMediaUtility::Instance() -> GetSystemAudioFormat());
			EMMediaMIDIEvent* opEvent = *oEventIterator;
			*(opEvent -> m_vpStart) += vOffset;
		}

		opProject -> GetMIDIEventRepository() -> SortEvents();
	}
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);

	m_opClip -> m_vShouldSeek = true;
	if(opNewTrack != NULL)
	{
		m_opRecentTrack = m_opClip -> GetTrack();
		m_opClip -> SetTrack(opNewTrack);
		
		;//cout_commented_out_4_release << "Clip " << m_opClip -> GetID() << " now has " << opNewTrack -> GetClipDataDestination() -> GetID() << " as destination!" << endl;
		
		if(vTrackWasEmpty)
		{
			opProject -> GetUnusedTracks() -> RemoveTrack(opNewTrack);
			opProject -> GetUsedTracks() -> AddTrack(opNewTrack);
		}	
		
		if(m_opRecentTrack -> CountClips() == 0)
		{
			opProject -> GetUsedTracks() -> RemoveTrack(m_opRecentTrack);
			opProject -> GetUnusedTracks() -> AddTrack(m_opRecentTrack);
		} 

		EMRealtimeOutputDescriptor* opOut = EMOutputRepository::Instance() -> FindClockMaster();
		if(opOut != NULL)
		{
			opOut -> RegisterAsClockMaster();
			EMMediaTimer::Instance() -> RegisterClockMaster(opOut);
		}

/*		if(m_opClip->HasEffectTrack())
		{
			//m_opClip -> GetDestination() -> SetDestination(opNewTrack -> GetTrackDestination() -> GetID());
			m_opClip -> GetDestination() -> SetDestination(opNewTrack -> GetClipDataDestination());
		}*/
	}



//	opTimer -> SeekToFrame(opTimer -> NowFrame());
	if(eMode == EM_PLAYING || eMode == EM_RECORDING)
		opTimer -> SetState((eMode == EM_RECORDING ? EM_COUNTING_IN_FOR_RECORDING : EM_COUNTING_IN_FOR_PLAYBACK));

	return NULL;
}

bool MediaCommandMoveClip::RequiresParameters()
{
	return true;
}

bool MediaCommandMoveClip::IsUndoable()
{
	return false;
}
 
EMCommand* MediaCommandMoveClip::CloneAndClear()
{
	return EM_new MediaCommandMoveClip(m_opClip, m_vOldStart, m_opRecentTrack);
}

void MediaCommandMoveClip::UndoE()
{
	if(m_opClip != NULL)
	{
//		m_opClip -> SetFirstFrame(m_vOldStart);
		m_opClip -> SetStart(m_vOldStart);
		if(m_opRecentTrack != NULL)
		{
			EMMediaEngine* opEngine = EMMediaEngine::Instance();
			EMMediaProject* opProject = opEngine -> GetMediaProject();
			EMMediaTrack* opCurrentTrack = m_opClip -> GetTrack();
			
			if(opCurrentTrack -> CountClips() == 1)
			{
				opProject -> GetUsedTracks() -> RemoveTrack(opCurrentTrack);
				opProject -> GetUnusedTracks() -> AddTrack(opCurrentTrack);
			}
			m_opClip -> SetTrack(m_opRecentTrack);
			opProject -> GetUnusedTracks() -> RemoveTrack(m_opRecentTrack);
			opProject -> GetUsedTracks() -> AddTrack(m_opRecentTrack);
		}
	}
}
