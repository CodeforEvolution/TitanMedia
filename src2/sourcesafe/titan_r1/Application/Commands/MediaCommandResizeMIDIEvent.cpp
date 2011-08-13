#include "MediaCommandResizeMIDIEvent.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMMediaEngine.h"
#include "EMMediaMIDIEvent.h"
#include "EMMediaMIDIEventPosition.h"
#include "EMMediaMIDIEventRepository.h"
#include "EMMediaProject.h"
#include "EMMIDIClip.h"
#include "EMMIDIGlobals.h"

MediaCommandResizeMIDIEvent::MediaCommandResizeMIDIEvent()
	:	m_opOnEvent(NULL),
		m_vOldStart(0),
		m_vOldDuration(0)
{
}

MediaCommandResizeMIDIEvent::MediaCommandResizeMIDIEvent(EMMediaMIDIEvent* p_opOldNoteOn, int64 p_vOldStart, int64 p_vOldDuration)
	:	m_opOnEvent(p_opOldNoteOn),
		m_vOldStart(p_vOldStart),
		m_vOldDuration(p_vOldDuration)
{
}

MediaCommandResizeMIDIEvent::~MediaCommandResizeMIDIEvent()
{
}

//Takes a EMMediaMIDIEvent pointer to a NOTE ON event,
//a starting time (in microseconds) - measured from song start ( =0)
//and a duration of the note, in microseconds - measured from the NOTE ON starting time.
void* MediaCommandResizeMIDIEvent::ExecuteE(void* p_upNoteOnEvent, void* p_upStart, void* p_upDuration)
{
	EMMediaMIDIEvent* opOnEvent = static_cast<EMMediaMIDIEvent*>(p_upNoteOnEvent);
	int64 vStart = *static_cast<int64*>(p_upStart);
	int64 vDuration = *static_cast<int64*>(p_upDuration);
	EMMIDIClip* opTouchedClip = NULL;

	if(opOnEvent -> GetBaseEventType() != EM_MIDI_EVENT_TYPE_NOTE_ON)
		return NULL;

	EMMediaMIDIEvent* opOffEvent = opOnEvent -> m_opNoteOff;
	if(opOffEvent == NULL)
		return NULL;

//	if(opOnEvent -> IsObjectDeleted() || opOffEvent -> IsObjectDeleted())
//	{
//		EMDebugger("ERROR! Attempted to (resize) move a NOTE ON and/or a NOTE OFF that was marked as deleted!");
//		return NULL;
//	}

	m_opOnEvent = opOnEvent;
	m_vOldStart = *opOnEvent -> m_vpStart;
	m_vOldDuration = *(opOnEvent -> m_opNoteOff -> m_vpStart) - m_vOldStart;

	if(vStart != *opOnEvent -> m_vpStart)
	{
		list<EMMediaMIDIEvent*> oList;
		oList.push_back(opOnEvent);
		int64 vOffset = vStart - (*opOnEvent -> m_vpStart);

		eo << "Resizing NOTE ON: Offset: " << vOffset << ", vOldStart = " << m_vOldStart << ", vOldDuration = " << m_vOldDuration << ef;
		EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(MEDIA_COMMAND_MOVE_MIDI_EVENTS, &oList, &vOffset, NULL);
		opTouchedClip = static_cast<EMMIDIClip*>(opOnEvent -> GetPosition() -> GetClip());
	}

	if(vDuration + vStart != (*opOffEvent -> m_vpStart))
	{
		list<EMMediaMIDIEvent*> oList;
		oList.push_back(opOffEvent);
		int64 vOffset = (vStart + vDuration) - (*opOffEvent -> m_vpStart);
		eo << "Resizing NOTE OFF: Offset: " << vOffset << ", vOldStart = " << m_vOldStart << ", vOldDuration = " << m_vOldDuration << ef;
		EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(MEDIA_COMMAND_MOVE_MIDI_EVENTS, &oList, &vOffset, NULL);
		opTouchedClip = static_cast<EMMIDIClip*>(opOnEvent -> GetPosition() -> GetClip());
	}

	//Sort the clip's and the project's event repository sinze we've altered the event start times and their internal order.
	if(opTouchedClip != NULL)
	{
		opTouchedClip -> SortEvents();
		EMMediaEngine::Instance() -> GetMediaProject() -> GetMIDIEventRepository() -> SortEvents();
	}

	return NULL;
}

bool MediaCommandResizeMIDIEvent::RequiresParameters()
{
	return true;
}

bool MediaCommandResizeMIDIEvent::IsUndoable()
{
	return false;
}

void MediaCommandResizeMIDIEvent::UndoE()
{
	ExecuteE(static_cast<void*>(m_opOnEvent), static_cast<void*>(&m_vOldStart), static_cast<void*>(&m_vOldDuration));
}

EMCommand* MediaCommandResizeMIDIEvent::CloneAndClear()
{
	return EM_new MediaCommandResizeMIDIEvent(m_opOnEvent, m_vOldStart, m_vOldDuration);
}
