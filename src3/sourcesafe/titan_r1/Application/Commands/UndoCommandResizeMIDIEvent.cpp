#include "UndoCommandResizeMIDIEvent.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMMediaMIDIEvent.h"
#include "EMView.h"
#include "MIDIGClipContainer.h"
#include "TrackView.h"

UndoCommandResizeMIDIEvent::UndoCommandResizeMIDIEvent(TrackView* p_opTrackView) :
m_opTrackView(p_opTrackView)
{
}

UndoCommandResizeMIDIEvent::UndoCommandResizeMIDIEvent(TrackView* p_opTrackView, MIDIGClipContainer* p_opClipContainer, EMMediaMIDIEvent* p_opEvent, int64 p_vOriginalStartPos, int64 p_vOriginalLength) :
m_opClipContainer(p_opClipContainer),
m_opEvent(p_opEvent),
m_opTrackView(p_opTrackView),
m_vOriginalStartPos(p_vOriginalStartPos),
m_vOriginalLength(p_vOriginalLength)
{
}

EMCommand* UndoCommandResizeMIDIEvent::CloneAndClear()
{
	return new UndoCommandResizeMIDIEvent(m_opTrackView, m_opClipContainer, m_opEvent, m_vOriginalStartPos, m_vOriginalLength);
}

void* UndoCommandResizeMIDIEvent::ExecuteE(void* upParameterOne, void* upParameterTwo, void* upParameterThree)
{
	m_opClipContainer = static_cast<MIDIGClipContainer*>(upParameterOne);
	m_opEvent = static_cast<EMMediaMIDIEvent*>(upParameterTwo);
	int64* vpValues = static_cast<int64*>(upParameterThree);
	m_vOriginalStartPos = vpValues[0];
	m_vOriginalLength = vpValues[1];
	return NULL;
}

const char* UndoCommandResizeMIDIEvent::GetShortDescription()
{
	return "Resize MIDI Event";
}

bool UndoCommandResizeMIDIEvent::IsUndoable()
{
	return true;
}

bool UndoCommandResizeMIDIEvent::RequiresParameters()
{
	return true;
}

// FIXME: Use the media command's own undo instead of "simulating" an undo with a new media command
void UndoCommandResizeMIDIEvent::UndoE()
{
	if(m_opTrackView -> GetView() -> LockWindow())
	{
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_RESIZE_MIDI_EVENT, m_opEvent, &m_vOriginalStartPos, &m_vOriginalLength);			
		m_opClipContainer -> UpdateSelection();
		m_opClipContainer -> UpdateEventsInClips();
		m_opTrackView -> GetView() -> Invalidate(m_opClipContainer -> Frame() & m_opTrackView -> GetView() -> Bounds());

		m_opTrackView -> GetView() -> UnlockWindow();
	}
}

