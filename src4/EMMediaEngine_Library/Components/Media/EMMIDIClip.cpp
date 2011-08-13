#include "EMRealtimeMIDIOutputDescriptor.h"
#include "EMMediaMIDIEventRepository.h"
#include "EMMediaMIDIEventPosition.h"
#include "EMMediaMIDIEventList.h"
#include "EMMediaMIDITrack.h"
#include "EMMediaProject.h"
#include "EMMediaUtility.h"
#include "EMMediaEngine.h"
#include "EMMediaTrack.h"
#include "EMMediaPool.h"
#include "EMMIDIClip.h"
#include "EMMIDIGlobals.h"

EMMIDIClip::EMMIDIClip(uint64 p_vStartFrame, EMMediaTrack* p_opBelongsToTrack)
	:	EMMediaClip(EM_TYPE_MIDI, "Recording", p_vStartFrame, p_opBelongsToTrack),
		m_vAllEventsAreMuted(false),
		m_opEventList(NULL)
{ 
	m_opEventList = EM_new EMMediaMIDIEventList();
}

EMMIDIClip::EMMIDIClip(EMMediaTrack* p_opOwner) 
	:	EMMediaClip(p_opOwner),
		m_vAllEventsAreMuted(false),
		m_opEventList(NULL)
{
	m_opEventList = EM_new EMMediaMIDIEventList();
}

EMMIDIClip::~EMMIDIClip()
{
	DiscardAllEvents();
}

EMMediaClip* EMMIDIClip::CloneClip(int64 p_vStartFrame, EMMediaTrack* p_opBelongsToTrack)
{
	if(p_opBelongsToTrack == NULL)
		EMDebugger("ERROR! Belongs to track is NULL");

	EMMediaMIDIEventRepository* opGlobalEventRepository = EMMediaEngine::Instance() -> GetMediaProject() -> GetMIDIEventRepository();
	
	EMMIDIClip* opClipClone = EM_new EMMIDIClip(GetType(), p_opBelongsToTrack);
	opClipClone -> SetMarkInLength(GetMarkInLength());
	opClipClone -> SetMarkOutLength(GetMarkOutLength());
	opClipClone -> SetStart(GetStart());
	opClipClone -> SetDestination(GetDestination());

	m_opEventList -> Lock();
	list<EMMediaMIDIEvent*> oOnList;
	EMMediaMIDIEventItem* opItem = m_opEventList -> FirstItem();

	//We must unlock the repository since GetStart tries to lock it...
	m_opEventList -> Unlock();

	int64 vTimeOffset = EMMediaUtility::Instance() -> FramesToTime(p_vStartFrame - GetStart(), EMMediaUtility::Instance() -> GetSystemAudioFormat());
	//Now lock it again!
	m_opEventList -> Lock();
	while(opItem != NULL)
	{
		if(! opItem -> m_opEvent -> IsObjectDeleted())
		{
			EMMediaMIDIEvent* opNewMIDIEventClone = EM_new EMMediaMIDIEvent(*opItem -> m_opEvent);
			*(opNewMIDIEventClone -> m_vpStart) = *(opItem -> m_opEvent -> m_vpStart) + vTimeOffset;
			
			opClipClone -> AddEvent(opNewMIDIEventClone);
			opNewMIDIEventClone -> GetPosition() -> SetClip(opClipClone);

			if(opItem -> m_opEvent -> m_vBaseType == EM_MIDI_EVENT_TYPE_NOTE_OFF)
			{
				list<EMMediaMIDIEvent*>::iterator oOnEvents;
				for(oOnEvents = oOnList.begin(); oOnEvents != oOnList.end(); oOnEvents++)
				{
					//If "this" original note ON's note off is this current event,
					if((*oOnEvents) -> m_opNoteOff == opItem -> m_opEvent)
					{
						//Then access the original note on's clone and set it's note off to point to this new clone
						(*oOnEvents) -> m_opClone -> m_opNoteOff = opNewMIDIEventClone;
						opGlobalEventRepository -> Store(opNewMIDIEventClone);
						opGlobalEventRepository -> Store((*oOnEvents) -> m_opClone);
						oOnList.remove(*oOnEvents);
						break;
					}
				}
			}
			else if(opItem -> m_opEvent -> m_vBaseType == EM_MIDI_EVENT_TYPE_NOTE_ON)
			{
				//Store the clone in the original
				opItem -> m_opEvent -> m_opClone = opNewMIDIEventClone;

				//And store the original event in a list
				oOnList.push_back(opItem -> m_opEvent);
			}
		}
		opItem = opItem -> m_opNext;
	}

	m_opEventList -> Unlock();

	return opClipClone;
}

bool EMMIDIClip::InitCheckE()
{
	return true;
}

EMMediaDataBuffer* EMMIDIClip::GetBuffer() const
{
	return EMMediaClip::GetBuffer();
//	if(! EMMediaBufferSource::m_vHasBeenInitialized)
//		MessageBox(NULL, "ERROR! Tried to get video buffer from buffer source before it was initialized!", "MediaEngine error!", MB_OK);
	
/*	EMMediaDataBuffer* opMediaDataBuffer = m_opBelongsToTrack -> GetBuffer();
	if(opMediaDataBuffer != NULL)
	{
		opMediaDataBuffer -> m_opDestination = this -> GetDestination();
		opMediaDataBuffer ->m_opStartDestination = this -> GetDestination();
		return opMediaDataBuffer; 
	}
	else return NULL; */
	return NULL;
}

int64 EMMIDIClip::GetActiveLength()
{
	m_opEventList -> Lock();
	if(m_opEventList -> Size() == 0)
	{
		m_opEventList -> Unlock();
		return 0;
	}

	uint64 vLastMIDIEventHitTime = (m_opEventList -> Last() == NULL ? -1 : *(m_opEventList -> Last() -> m_vpStart));
	uint64 vFirstMIDIEventHitTime = (m_opEventList -> First() == NULL ? -1 : *(m_opEventList -> First() -> m_vpStart));
	m_opEventList -> Unlock();
	return EMMediaUtility::Instance() -> TimeToFrames(vLastMIDIEventHitTime - vFirstMIDIEventHitTime, EMMediaUtility::Instance() -> GetSystemAudioFormat());
}

void EMMIDIClip::SetStart(int64 p_vStart)
{
	int64 vOldStartingTime = GetStart();
	EMMediaClip::SetStart(p_vStart);

	//Calculate how much the events should be moved...
	int64 vOffsetMicroseconds = EMMediaUtility::Instance() -> FramesToTime(p_vStart - vOldStartingTime, EMMediaUtility::Instance() -> GetSystemAudioFormat());

#ifdef DEBUG
	opEvents -> Display();
#endif
}

int64 EMMIDIClip::GetStart()
{
	m_opEventList -> Lock();
	if(m_opEventList -> Size() == 0)
	{
		m_opEventList -> Unlock();
		return 0;
	}
	uint64 vFirstMIDIEventHitTime = (m_opEventList -> First() == NULL ? -1 : *(m_opEventList -> First() -> m_vpStart));
	m_opEventList -> Unlock();
	return EMMediaUtility::Instance() -> TimeToFrames(vFirstMIDIEventHitTime, EMMediaUtility::Instance() -> GetSystemAudioFormat());
}

int64 EMMIDIClip::GetStop()
{
	m_opEventList -> Lock();
	if(m_opEventList -> Size() == 0)
	{
		m_opEventList -> Unlock();
		return 0;
	}
	uint64 vLastMIDIEventHitTime = (m_opEventList -> Last() == NULL ? -1 : *(m_opEventList -> Last() -> m_vpStart));
	m_opEventList -> Unlock();
	return EMMediaUtility::Instance() -> TimeToFrames(vLastMIDIEventHitTime, EMMediaUtility::Instance() -> GetSystemAudioFormat());
}

void EMMIDIClip::AddEvent(EMMediaMIDIEvent* p_opEvent)
{
	m_opEventList -> Lock();
	m_opEventList -> Store(p_opEvent);
	m_opEventList -> Unlock();
}

void EMMIDIClip::RemoveEvent(EMMediaMIDIEvent* p_opEvent)
{
	m_opEventList -> Lock();
	m_opEventList -> Remove(p_opEvent);
	m_opEventList -> Unlock();
}

int64 EMMIDIClip::CountEvents()
{
	m_opEventList -> Lock();
	int64 vSize = 0;
	EMMediaMIDIEventItem* opItem = m_opEventList -> FirstItem();
	while(opItem != NULL)
	{
		if(! opItem -> m_opEvent -> IsObjectDeleted())
			vSize++;
		opItem = opItem -> m_opNext;
	}
	m_opEventList -> Unlock();
	return vSize;
}

void EMMIDIClip::DiscardAllEvents()
{
	m_opEventList -> Lock();
	m_opEventList -> Clear();
	m_opEventList -> Unlock();
}

void EMMIDIClip::PurgeEvents()
{
	EMDebugger("Not implemented");
}

void EMMIDIClip::GetAllMIDIEvents(list<EMMediaMIDIEvent*>* p_opListToAddTo)
{
	m_opEventList -> Lock();
	EMMediaMIDIEventItem* opItem = m_opEventList -> FirstItem();
	while(opItem != NULL)
	{
		p_opListToAddTo -> push_back(opItem -> m_opEvent);
		opItem = opItem -> m_opNext;
	}
	m_opEventList -> Unlock();
}

void EMMIDIClip::GetActiveMIDIEvents(list<EMMediaMIDIEvent*>* p_opListToAddTo)
{
	m_opEventList -> Lock();
	EMMediaMIDIEventItem* opItem = m_opEventList -> FirstItem();
	while(opItem != NULL)
	{
		if(! opItem -> m_opEvent -> IsObjectDeleted())
			p_opListToAddTo -> push_back(opItem -> m_opEvent);
		opItem = opItem -> m_opNext;
	}
	m_opEventList -> Unlock();
}

void EMMIDIClip::GetDeletedMIDIEvents(list<EMMediaMIDIEvent*>* p_opListToAddTo)
{
	m_opEventList -> Lock();
	EMMediaMIDIEventItem* opItem = m_opEventList -> FirstItem();
	while(opItem != NULL)
	{
		if(opItem -> m_opEvent -> IsObjectDeleted())
			p_opListToAddTo -> push_back(opItem -> m_opEvent);
		opItem = opItem -> m_opNext;
	}
	m_opEventList -> Unlock();
}

void EMMIDIClip::SetTrack(EMMediaTrack* p_opBelongsToTrack)
{

//	_ASSERT((p_opBelongsToTrack -> GetType() & EM_TYPE_MIDI) > 0);
	EMMediaClip::SetTrack(p_opBelongsToTrack);

	m_opEventList -> Lock();
	EMMediaMIDIEventItem* opItem = m_opEventList -> FirstItem();
	while(opItem != NULL)
	{
		opItem -> m_opEvent -> GetPosition() -> SetDestination(static_cast<EMRealtimeMIDIOutputDescriptor*>(p_opBelongsToTrack -> GetDestination()));
		opItem -> m_opEvent -> GetPosition() -> SetChannel(static_cast<EMMediaMIDITrack*>(p_opBelongsToTrack) -> GetMIDIChannel());
		opItem = opItem -> m_opNext;
	}
	m_opEventList -> Unlock();
}

void EMMIDIClip::SortEvents()
{
	m_opEventList -> Sort();
}

void EMMIDIClip::SetMuted(bool p_vAllEventsMuted)
{
	m_vAllEventsAreMuted = p_vAllEventsMuted;
/*	m_opEventList -> Lock();
	EMMediaMIDIEventItem* opItem = m_opEventList -> FirstItem();
	while(opItem != NULL)
	{
		opItem -> m_opEvent -> SetMuted(m_vAllEventsAreMuted);
		opItem = opItem -> m_opNext;
	}
	m_opEventList -> Unlock(); */
}

bool EMMIDIClip::IsMuted() const
{
	return m_vAllEventsAreMuted;
}

void EMMIDIClip::SetDeleted(bool p_vFlag)
{
	EMMediaClip::SetDeleted(p_vFlag);
//	m_opEventList -> Lock();
//	EMMediaMIDIEventItem* opItem = m_opEventList -> FirstItem();
//	while(opItem != NULL)
//	{
//		opItem -> m_opEvent -> SetDeleted(p_vFlag);
//		opItem = opItem -> m_opNext;
//	}
//	m_opEventList -> Unlock();
}