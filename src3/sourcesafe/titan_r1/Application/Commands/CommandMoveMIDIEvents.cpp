#include "CommandMoveMIDIEvents.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMMediaMIDIEvent.h"
#include "EMView.h"
#include "MIDIGClipContainer.h"
#include "TrackView.h"
#include "TrackWindowStatusBar.h"

#include <string>

CommandMoveMIDIEvents::CommandMoveMIDIEvents(TrackView* p_opTrackView) :
m_opTrackView(p_opTrackView)
{
}

CommandMoveMIDIEvents::CommandMoveMIDIEvents(TrackView* p_opTrackView, MIDIGClipContainer* p_opClipContainer, list<EMMediaMIDIEvent*> p_oMovedEvents, int32 p_vKeyOffset, int64 p_vPositionOffset) :
m_oMovedEvents(p_oMovedEvents),
m_opClipContainer(p_opClipContainer),
m_opTrackView(p_opTrackView),
m_vPositionOffset(p_vPositionOffset),
m_vKeyOffset(p_vKeyOffset)
{
}

CommandMoveMIDIEvents::~CommandMoveMIDIEvents()
{
}

EMCommand* CommandMoveMIDIEvents::CloneAndClear()
{
	CommandMoveMIDIEvents* opCommand = new CommandMoveMIDIEvents(m_opTrackView, m_opClipContainer, m_oMovedEvents, m_vKeyOffset, m_vPositionOffset);
	m_oMovedEvents.clear();
	return opCommand;
}

void CommandMoveMIDIEvents::ExecuteBody(list<EMMediaMIDIEvent*>* p_opMovedEvents, int32 p_vKeyOffset, int64 p_vPositionOffset)
{
	// First move the time positions
	EMCommandRepository::Instance() -> ExecuteCommandNoUndo(MEDIA_COMMAND_MOVE_MIDI_EVENTS, p_opMovedEvents, &p_vPositionOffset);
	delete EMCommandRepository::Instance() -> GetUndoClone(MEDIA_COMMAND_MOVE_MIDI_EVENTS);

	// Now move the key positions
	if(p_vKeyOffset != 0)
	{
		list<EMMediaMIDIEvent*>::iterator oIterator = p_opMovedEvents -> begin();
		while(oIterator != p_opMovedEvents -> end())
		{
			if((*oIterator) -> m_vBaseType == 144 || (*oIterator) -> m_vBaseType == 128)
				(*oIterator) -> m_vData1 += p_vKeyOffset;
			oIterator++;
		}
	}

	m_opClipContainer = static_cast<MIDIGClipContainer*>(EMGClipContainer::GetClipContainerWithSelection());
	m_opClipContainer -> UpdateEventsInClips();

	char vOutput[80];
	if(m_oMovedEvents.size() == 1)
		sprintf(vOutput, "Event moved, Offset: %ld, %lld", p_vKeyOffset, p_vPositionOffset);
	else
		sprintf(vOutput, "%ld events moved, Offset: %ld, %lld", m_oMovedEvents.size(), p_vKeyOffset, p_vPositionOffset);
	TrackWindowStatusBar::Instance() -> WriteMessage(string(vOutput));
}

void* CommandMoveMIDIEvents::ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree)
{
	m_vKeyOffset = *(static_cast<int32*>(p_opParameterOne));
	m_vPositionOffset = *(static_cast<int64*>(p_opParameterTwo));
	m_oMovedEvents = *(static_cast<list<EMMediaMIDIEvent*>*>(p_opParameterThree));

	ExecuteBody(&m_oMovedEvents, m_vKeyOffset, m_vPositionOffset);
	return NULL;
}

const char* CommandMoveMIDIEvents::GetShortDescription()
{
	return "Move MIDI Events";
}

bool CommandMoveMIDIEvents::IsUndoable()
{
	return true;
}

void CommandMoveMIDIEvents::RedoE()
{
//	ExecuteBody(m_oMovedClips, m_vTrackOffset, m_vPositionOffset);
}

bool CommandMoveMIDIEvents::RequiresParameters()
{
	return true;
}

void CommandMoveMIDIEvents::UndoE()
{
	if(m_opTrackView -> GetView() -> LockWindow())
	{
		ExecuteBody(&m_oMovedEvents, -1 * m_vKeyOffset, -1 * m_vPositionOffset);
		m_opClipContainer -> UpdateSelection();
		m_opTrackView -> GetView() -> Invalidate(m_opClipContainer -> Frame() & m_opTrackView -> GetView() -> Bounds());

		m_opTrackView -> GetView() -> UnlockWindow();
	}
}
