#include "MediaInfoCommandGetMIDIEventsForTrack.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMIDIClip.h"
#include "EMMediaClipRepository.h"
#include "EMMediaMIDIEvent.h"
#include "EMMediaTrack.h"
#include "EMMediaMIDITrack.h"
#include "EMMediaTrackRepository.h"
#include "EMMediaMIDIEventList.h"

MediaInfoCommandGetMIDIEventsForTrack::MediaInfoCommandGetMIDIEventsForTrack()
{
	m_opSortedEventList = EM_new EMMediaMIDIEventList();
}

MediaInfoCommandGetMIDIEventsForTrack::~MediaInfoCommandGetMIDIEventsForTrack()
{
	m_opSortedEventList -> Lock();
	m_opSortedEventList -> Clear();
	m_opSortedEventList -> Clear();
	delete m_opSortedEventList;
	m_oReturnValue.clear();
}

void* MediaInfoCommandGetMIDIEventsForTrack::ExecuteE(void* p_upMediaTrackID, void*, void*)
{
	int32 vTrackID = *static_cast<int32*>(p_upMediaTrackID);

	m_oReturnValue.clear();

	m_opSortedEventList -> Lock();
	m_opSortedEventList -> Clear();

	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();
	
	EMMediaClipRepository* opClips = opProject -> GetMIDIClipRepository();
	EMMediaTrackRepository* opUsed = opProject -> GetUsedTracks();
	EMMediaTrackRepository* opUnused = opProject -> GetUnusedTracks();

	opUsed -> LockContainer();
	EMMediaMIDITrack* opTrack = static_cast<EMMediaMIDITrack*>(opUsed -> Find(vTrackID));
	opUsed -> UnlockContainer();

	if(opTrack == NULL)
	{
		opUnused -> LockContainer();
		opTrack = static_cast<EMMediaMIDITrack*>(opUnused -> Find(vTrackID));
		opUnused -> UnlockContainer();
	}

	if(opTrack == NULL)
		EMDebugger("ERROR! Couldn't find that track ID!");

	

	//Loop through the clip repository, and for each clip that belongs to "this" track, get the event list, and then later, sort them by puting them
	//into the sorted repository that we use to return.
	opClips -> LockContainer();
	opClips -> Rewind();
	list<EMMediaMIDIEvent*> oUnorderedEventList;
	while(opClips -> Current() != NULL)
	{
		if(opClips -> Current() -> GetTrack() == opTrack)
			static_cast<EMMIDIClip*>(opClips -> Current()) -> GetAllMIDIEvents(&oUnorderedEventList);
		opClips -> Next();
	}
	opClips -> UnlockContainer();
	list<EMMediaMIDIEvent*>::const_iterator oEventIterator;
	for(oEventIterator = oUnorderedEventList.begin(); oEventIterator != oUnorderedEventList.end(); oEventIterator++)
	{
		if(! (*oEventIterator) -> IsObjectDeleted())
			m_opSortedEventList -> Store(*oEventIterator);
	}

/*	EMMediaMIDIEvent* opEvent = NULL;
	m_opSortedEventList -> Reset();
	while((opEvent = (*m_opSortedEventList)[0]) != NULL) //Keep accessing event at time 0, since that will enumerate the events.*/

	EMMediaMIDIEventItem* opItem = m_opSortedEventList -> FirstItem();
	while(opItem != NULL)
	{
		m_oReturnValue.push_back(opItem -> m_opEvent);
		opItem = opItem -> m_opNext;
	}
	m_opSortedEventList -> Reset();
	m_opSortedEventList -> Unlock();
	return static_cast<void*>(&m_oReturnValue);
}

bool MediaInfoCommandGetMIDIEventsForTrack::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetMIDIEventsForTrack::IsUndoable()
{
	return false;
}