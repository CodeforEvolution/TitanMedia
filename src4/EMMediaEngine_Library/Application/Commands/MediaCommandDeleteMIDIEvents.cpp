#include "MediaCommandDeleteMIDIEvents.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMIDIClip.h"
#include "EMMIDIGlobals.h"
#include "EMMediaMIDIEvent.h"
#include "EMMediaMIDIEventPosition.h"
#include "EMMediaMIDIEventRepository.h"
#include "EMMediaUtility.h"
#include "EMCommandRepository.h"
#include "MediaCommandDeleteClip.h"
#include "EMMediaTrackRepository.h"
#include "CommandIDs.h"

MediaCommandDeleteMIDIEvents::MediaCommandDeleteMIDIEvents()
{
}

MediaCommandDeleteMIDIEvents::MediaCommandDeleteMIDIEvents(list<EMMediaMIDIEvent*>* p_opDeletedEvents)
	:	m_oDeletedEvents(*p_opDeletedEvents)
{
}

MediaCommandDeleteMIDIEvents::~MediaCommandDeleteMIDIEvents()
{
	m_oDeletedEvents.clear();
}

void* MediaCommandDeleteMIDIEvents::ExecuteE(void* p_upMIDIEventList, void*, void*)
{
	list<EMMediaMIDIEvent*>* opEventList = static_cast<list<EMMediaMIDIEvent*>*>(p_upMIDIEventList);

	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();

	m_oDeletedEvents.clear();
	m_oReturnValue.clear();
	list<EMMIDIClip*> oTouchedClips; 
	list<EMMediaMIDIEvent*>::iterator oIterator;
	for(oIterator = opEventList -> begin(); oIterator != opEventList -> end(); oIterator++)
	{
		EMMediaMIDIEvent* opEvent = (*oIterator);
		EMMIDIClip* opClip = static_cast<EMMIDIClip*>(opEvent -> GetPosition() -> GetClip());
		if(find(oTouchedClips.begin(), oTouchedClips.end(), static_cast<EMMIDIClip*>(opEvent -> GetPosition() -> GetClip())) == oTouchedClips.end())
			oTouchedClips.push_back(static_cast<EMMIDIClip*>(opEvent -> GetPosition() -> GetClip()));

		//Don't delete the very last event! Let the MediaCommandDeleteClip do that for us...
		//(or we'll be in trouble because of calls to MediaInfoCommandGetClipDuration and others that try to access the events in a MIDI clip...
		if(opClip -> CountEvents() > 1 && opEvent -> GetBaseEventType() != EM_MIDI_EVENT_TYPE_NOTE_OFF)
			opEvent -> SetDeleted(true); 
		
		m_oDeletedEvents.push_back(opEvent);
		
	}

	while(oTouchedClips.size() > 0)
	{
		if(oTouchedClips.front() -> CountEvents() <= 1)
		{
			int32 vClipID = oTouchedClips.front() -> GetID();
			
			//And don't actually delete the clip (if necessary). 
			//Instead, assume the GUI does the call to MediaCommandDeleteClip
			m_oReturnValue.push_back(oTouchedClips.front() -> GetID()); 
		}
		else
			oTouchedClips.front() -> SortEvents();
		oTouchedClips.pop_front();
	}
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
	oTouchedClips.clear();
	opProject -> GetMIDIEventRepository() -> SortEvents();
	return static_cast<void*>(&m_oReturnValue);
}

bool MediaCommandDeleteMIDIEvents::RequiresParameters()
{
	return true;
}

bool MediaCommandDeleteMIDIEvents::IsUndoable()
{
	return false;
}

EMCommand* MediaCommandDeleteMIDIEvents::CloneAndClear()
{
	return EM_new MediaCommandDeleteMIDIEvents(&m_oDeletedEvents);
}

void MediaCommandDeleteMIDIEvents::UndoE()
{
	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();

	list<EMMIDIClip*> oReincarnatedClips; 
	list<EMMIDIClip*> oTouchedClips; 
	list<EMMediaMIDIEvent*>::iterator oIterator;

	//First, we need to mark all the respective events as NOT deleted (m_vIsDeleted = false).
	//At the same time, we store the event's clip in one list for sorting and stuff when we're done,
	//and in another list - if the clip was marked as deleted - for "reincarnation" when we're done with undeleting the events.
	for(oIterator = m_oDeletedEvents.begin(); oIterator != m_oDeletedEvents.end(); oIterator++)
	{
		EMMediaMIDIEvent* opEvent = (*oIterator);
		EMMIDIClip* opClip = static_cast<EMMIDIClip*>(opEvent -> GetPosition() -> GetClip());
		if(find(oReincarnatedClips.begin(), oReincarnatedClips.end(), static_cast<EMMIDIClip*>(opEvent -> GetPosition() -> GetClip())) == oReincarnatedClips.end() &&
			static_cast<EMMIDIClip*>(opEvent -> GetPosition() -> GetClip()) -> IsObjectDeleted())
		{
			oReincarnatedClips.push_back(static_cast<EMMIDIClip*>(opEvent -> GetPosition() -> GetClip()));
		}
		if(find(oTouchedClips.begin(), oTouchedClips.end(), static_cast<EMMIDIClip*>(opEvent -> GetPosition() -> GetClip())) == oTouchedClips.end() &&
			static_cast<EMMIDIClip*>(opEvent -> GetPosition() -> GetClip()) -> IsObjectDeleted())
		{
			oTouchedClips.push_back(static_cast<EMMIDIClip*>(opEvent -> GetPosition() -> GetClip()));
		}
		opEvent -> SetDeleted(false);
	}

	//This list contains only clips that previously were marked as deleted. So, we need to undelete them, since we've undeleted their event(s). 
	//We can use (i think) the UndoE method in the MediaCommandDeleteClip command... (Hopefully!)
	while(oReincarnatedClips.size() > 0)
	{
		EMMIDIClip* opClipToRevive = oReincarnatedClips.front();
		if(opClipToRevive != NULL)
		{
			MediaCommandDeleteClip* opCommand = EM_new MediaCommandDeleteClip(opClipToRevive -> GetID(), -1, NULL);
			opCommand -> UndoE();

			if(opClipToRevive -> GetTrack() -> CountClips() <= 0 && opClipToRevive -> GetTrack() -> GetType() != EM_TYPE_TRANSITION)
			{
				opProject -> GetUsedTracks() -> RemoveTrack(opClipToRevive -> GetTrack());
				opProject -> GetUnusedTracks() -> AddTrack(opClipToRevive -> GetTrack());
			}
		}
		oReincarnatedClips.pop_front();
	}
	oReincarnatedClips.clear();

	//Now that we're done and have all the events and the clips again, we need to 
	//sort the clip's event lists, and the global event list as well!
	while(oTouchedClips.size() > 0)
	{
		EMMIDIClip* opClipToSort = oTouchedClips.front();
		if(opClipToSort != NULL)
		{
			opClipToSort -> SortEvents();
		}
		oTouchedClips.pop_front();
	}
	oTouchedClips.clear();
	
	opProject -> GetMIDIEventRepository() -> SortEvents();
}