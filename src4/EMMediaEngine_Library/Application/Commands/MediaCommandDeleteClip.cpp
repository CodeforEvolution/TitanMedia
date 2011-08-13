#include "MediaCommandDeleteClip.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaClipRepository.h"
#include "EMMediaTimer.h"
#include "EMMediaTrackRepository.h"
#include "EMBeVideoConsumerNode.h"
#include "EMMediaPoolEntry.h"
#include "EMMediaPool.h"
#include "EMFileInputDescriptor.h"
#include "EMMediaEffectTrackRepository.h"
#include "EMMediaEffectTrack.h"
#include "EMMIDIGlobals.h"
#include "EMMIDIClip.h"

MediaCommandDeleteClip::MediaCommandDeleteClip()
	:	m_vDeletedClipID(-1),
		m_vFxID(-1)
{
}

MediaCommandDeleteClip::MediaCommandDeleteClip(int32 p_vDeletedClipID, int32 p_vFxID, const list<EMMediaMIDIEvent*>* p_opDeletedMIDIEvents)
	:	m_vDeletedClipID(p_vDeletedClipID),
		m_vFxID(p_vFxID)
{
	if(p_opDeletedMIDIEvents != NULL)
		m_oDeletedMIDIEvents = *p_opDeletedMIDIEvents;
}

void* MediaCommandDeleteClip::ExecuteE(void* p_upMediaClipID, void*, void*)
{
	int32 vMediaClipID = *static_cast<int32*>(p_upMediaClipID);
	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();
	EMMediaClipRepository* opAudioClips = opProject -> GetAudioClipRepository();
	EMMediaClipRepository* opVideoClips = opProject -> GetVideoClipRepository();
	EMMediaClipRepository* opVideoTransitionClips = opProject -> GetVideoTransitionClipRepository();
	EMMediaClipRepository* opMIDIClips = opProject -> GetMIDIClipRepository();
	EMMediaTimer* opTimer = EMMediaTimer::Instance();
	EMMediaClip* opClip = NULL;
	int32 vFxID = -1;
	 
	if(opProject -> LockVideo()) 
	{
		if(opProject -> LockAudio())
		{
			opAudioClips -> LockContainer();
			opVideoClips -> LockContainer(); 
			opVideoTransitionClips -> LockContainer(); 
			opMIDIClips -> LockContainer(); 
			if((opClip = opAudioClips -> Find(vMediaClipID)) != NULL)
			{
				if(opClip -> IsObjectDeleted())
				{
					opAudioClips -> UnlockContainer();
					opVideoClips -> UnlockContainer();
					opVideoTransitionClips -> UnlockContainer();
					opMIDIClips -> UnlockContainer();
					opProject -> UnlockAudio();
					opProject -> UnlockVideo();
					return NULL;
				}
				opClip -> SetDeleted(true); //Instead of removing it, just "hide" it, for later purge/destroy!
				EMMediaPoolEntry* opEntry = opProject -> GetMediaPool() -> Find(opClip -> GetDescriptor() -> GetMediaPoolEntryID());
				opEntry -> DecreaseUsed();

				m_vDeletedClipID = opClip -> GetID();
				EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);

				opAudioClips -> UnlockContainer();
				opVideoClips -> UnlockContainer();
				opVideoTransitionClips -> UnlockContainer();
				opMIDIClips -> UnlockContainer();
			}
			else if((opClip = opVideoClips -> Find(vMediaClipID)) != NULL)
			{
				if(opClip -> IsObjectDeleted())
				{
					opAudioClips -> UnlockContainer();
					opVideoClips -> UnlockContainer();
					opVideoTransitionClips -> UnlockContainer();
					opMIDIClips -> UnlockContainer();
					opProject -> UnlockAudio();
					opProject -> UnlockVideo();
					return NULL;
				}
				opClip -> SetDeleted(true); //Instead of removing it, just "hide" it, for later purge/destroy!
				EMMediaPoolEntry* opEntry = opProject -> GetMediaPool() -> Find(opClip -> GetDescriptor() -> GetMediaPoolEntryID());
				opEntry -> DecreaseUsed();
				
				m_vDeletedClipID = opClip -> GetID();
				EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);

				opAudioClips -> UnlockContainer();
				opVideoClips -> UnlockContainer();
				opVideoTransitionClips -> UnlockContainer();
				opMIDIClips -> UnlockContainer();
			}
			else if((opClip = opMIDIClips -> Find(vMediaClipID)) != NULL)
			{
				if(opClip -> IsObjectDeleted())
				{
					opAudioClips -> UnlockContainer();
					opVideoClips -> UnlockContainer();
					opVideoTransitionClips -> UnlockContainer();
					opMIDIClips -> UnlockContainer();
					opProject -> UnlockAudio();
					opProject -> UnlockVideo();
					return NULL;
				}
				opClip -> SetDeleted(true); //Instead of removing it, just "hide" it, for later purge/destroy!
				
				m_oDeletedMIDIEvents.clear();
				static_cast<EMMIDIClip*>(opClip) -> GetActiveMIDIEvents(&m_oDeletedMIDIEvents);
				list<EMMediaMIDIEvent*>::iterator oIter;
				for(oIter = m_oDeletedMIDIEvents.begin(); oIter != m_oDeletedMIDIEvents.end(); oIter++)
					if((*oIter) -> GetBaseEventType() != EM_MIDI_EVENT_TYPE_NOTE_OFF)
						(*oIter) -> SetDeleted(true);

				m_vDeletedClipID = opClip -> GetID();
				EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);

				opAudioClips -> UnlockContainer();
				opVideoClips -> UnlockContainer();
				opVideoTransitionClips -> UnlockContainer();
				opMIDIClips -> UnlockContainer();
			}
			else if((opClip = opVideoTransitionClips -> Find(vMediaClipID)) != NULL)
			{
				if(opClip -> IsObjectDeleted())
				{
					opAudioClips -> UnlockContainer();
					opVideoClips -> UnlockContainer();
					opVideoTransitionClips -> UnlockContainer();
					opMIDIClips -> UnlockContainer();
					opProject -> UnlockAudio();
					opProject -> UnlockVideo();
					return NULL;
				}
				opClip -> SetDeleted(true);

				m_vDeletedClipID = opClip -> GetID();
				EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);

				opAudioClips -> UnlockContainer();
				opVideoClips -> UnlockContainer();
				opVideoTransitionClips -> UnlockContainer();
				opMIDIClips -> UnlockContainer();
			}
			else
			{
				opAudioClips -> UnlockContainer();
				opVideoClips -> UnlockContainer();
				opVideoTransitionClips -> UnlockContainer();
				opMIDIClips -> UnlockContainer();
			}
			
			if(opClip != NULL)
			{
				if(opClip -> GetTrack() -> CountClips() <= 0 && opClip -> GetTrack() -> GetType() != EM_TYPE_TRANSITION)
				{
					opProject -> GetUsedTracks() -> RemoveTrack(opClip -> GetTrack());
					opProject -> GetUnusedTracks() -> AddTrack(opClip -> GetTrack());
				}

				if(opClip -> GetEffectTrackID() != -1)
				{
					m_vFxID = opClip -> GetDestination() -> GetID();
					static_cast<EMMediaEffectTrack*>(opClip -> GetDestination()) -> SetDeleted(true);
				}
				
			}
			opProject -> UnlockAudio();
		}
		opProject -> UnlockVideo();


		EMRealtimeOutputDescriptor* opOut = EMOutputRepository::Instance() -> FindClockMaster();
		if(opOut != NULL)
		{
			opOut -> RegisterAsClockMaster();
			EMMediaTimer::Instance() -> RegisterClockMaster(opOut);
		}

		EMMediaTimerState eMode = opTimer -> GetState();
		if(eMode == EM_PLAYING || eMode == EM_RECORDING)
			opTimer -> SetState(EM_STOPPED);
		opTimer -> SeekToFrame(opTimer -> NowFrame());
		if(eMode == EM_PLAYING || eMode == EM_RECORDING)
			opTimer -> SetState((eMode == EM_RECORDING ? EM_COUNTING_IN_FOR_RECORDING : EM_COUNTING_IN_FOR_PLAYBACK));
	}
	return NULL;
}

bool MediaCommandDeleteClip::RequiresParameters()
{
	return true;
}

bool MediaCommandDeleteClip::IsUndoable()
{
	return false;
}

EMCommand* MediaCommandDeleteClip::CloneAndClear()
{
	return EM_new MediaCommandDeleteClip(m_vDeletedClipID, m_vFxID, &m_oDeletedMIDIEvents);
}

void MediaCommandDeleteClip::UndoE()
{
	if(m_vDeletedClipID != -1)
	{
		EMMediaEngine* opEngine = EMMediaEngine::Instance();
		EMMediaProject* opProject = opEngine -> GetMediaProject();
		EMMediaClipRepository* opAudioClips = opProject -> GetAudioClipRepository();
		EMMediaClipRepository* opVideoClips = opProject -> GetVideoClipRepository();
		EMMediaClipRepository* opVideoTransitionClips = opProject -> GetVideoTransitionClipRepository();
		EMMediaClipRepository* opMIDIClips = opProject -> GetMIDIClipRepository();
		EMMediaTimer* opTimer = EMMediaTimer::Instance();
		EMMediaClip* opClip = NULL;
		
		if(opProject -> LockVideo())
		{
			if(opProject -> LockAudio())
			{
				opAudioClips -> LockContainer();
				opVideoClips -> LockContainer();
				opVideoTransitionClips -> LockContainer(); 
				opMIDIClips -> LockContainer();
				if((opClip = opAudioClips -> Find(m_vDeletedClipID)) != NULL)
				{
					bool vIsDeleted = opClip -> IsObjectDeleted();
					opClip -> SetDeleted(! opClip -> IsObjectDeleted());
					m_vDeletedClipID = m_vDeletedClipID;

					EMMediaPoolEntry* opEntry = opProject -> GetMediaPool() -> Find(opClip -> GetDescriptor() -> GetMediaPoolEntryID());
					if(vIsDeleted)
						opEntry -> IncreaseUsed();
					else
						opEntry -> DecreaseUsed();

					opAudioClips -> UnlockContainer();
					opVideoClips -> UnlockContainer();
					opVideoTransitionClips -> UnlockContainer();
					opMIDIClips -> UnlockContainer();
				}
				else if((opClip = opVideoClips -> Find(m_vDeletedClipID)) != NULL)
				{
					bool vIsDeleted = opClip -> IsObjectDeleted();
					opClip -> SetDeleted(! opClip -> IsObjectDeleted());
					m_vDeletedClipID = m_vDeletedClipID;

					EMMediaPoolEntry* opEntry = opProject -> GetMediaPool() -> Find(opClip -> GetDescriptor() -> GetMediaPoolEntryID());
					if(vIsDeleted)
						opEntry -> IncreaseUsed();
					else
						opEntry -> DecreaseUsed();

					opAudioClips -> UnlockContainer();
					opVideoClips -> UnlockContainer();
					opVideoTransitionClips -> UnlockContainer();
					opMIDIClips -> UnlockContainer();
				}
				else if((opClip = opMIDIClips -> Find(m_vDeletedClipID)) != NULL)
				{
					bool vIsDeleted = opClip -> IsObjectDeleted();
					opClip -> SetDeleted(false);

					list<EMMediaMIDIEvent*>::iterator oIter;
					for(oIter = m_oDeletedMIDIEvents.begin(); oIter != m_oDeletedMIDIEvents.end(); oIter++)
						if((*oIter) -> GetBaseEventType() != EM_MIDI_EVENT_TYPE_NOTE_OFF)
							(*oIter) -> SetDeleted(false);
					
					opAudioClips -> UnlockContainer();
					opVideoClips -> UnlockContainer();
					opVideoTransitionClips -> UnlockContainer();
					opMIDIClips -> UnlockContainer();
				}
				else if((opClip == opVideoTransitionClips -> Find(m_vDeletedClipID)) != NULL)
				{
					bool vIsDeleted = opClip -> IsObjectDeleted();
					opClip -> SetDeleted(! opClip -> IsObjectDeleted());
					m_vDeletedClipID = m_vDeletedClipID;

					opAudioClips -> UnlockContainer();
					opVideoClips -> UnlockContainer();
					opVideoTransitionClips -> UnlockContainer();
					opMIDIClips -> UnlockContainer();
				}
				else
				{
					opAudioClips -> UnlockContainer(); 
					opVideoClips -> UnlockContainer();
					opVideoTransitionClips -> UnlockContainer();
					opMIDIClips -> UnlockContainer();
				}
				
				if(opClip != NULL)
				{
					if(opClip -> GetTrack() -> CountClips() == 1)
					{
						opProject -> GetUsedTracks() -> AddTrack(opClip -> GetTrack());
						opProject -> GetUnusedTracks() -> RemoveTrack(opClip -> GetTrack());
					}
				}
				opProject -> UnlockAudio();
			}

			if(m_vFxID != -1)
			{
				static_cast<EMMediaEffectTrack*>(opClip -> GetDestination()) -> SetDeleted(true);
			}

			opProject -> UnlockVideo();
			EMMediaTimerState eMode = opTimer -> GetState();
			if(eMode == EM_PLAYING || eMode == EM_RECORDING)
				opTimer -> SetState(EM_STOPPED);
			opTimer -> SeekToFrame(opTimer -> NowFrame());
			if(eMode == EM_PLAYING || eMode == EM_RECORDING)
				opTimer -> SetState((eMode == EM_RECORDING ? EM_COUNTING_IN_FOR_RECORDING : EM_COUNTING_IN_FOR_PLAYBACK));
		}
	}
}
