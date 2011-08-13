#include "CommandCutMIDIEvents.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMMediaMIDIEvent.h"
#include "EMView.h"
#include "MIDIGClipContainer.h"
#include "TrackView.h"
#include "TrackWindowStatusBar.h"

#include <string>

CommandCutMIDIEvents::CommandCutMIDIEvents(TrackView* p_opTrackView) :
m_opTrackView(p_opTrackView)
{
}

CommandCutMIDIEvents::CommandCutMIDIEvents(TrackView* p_opTrackView, MIDIGClipContainer* p_opClipContainer, list<EMMediaMIDIEvent*> p_oCutEvents, int32 p_vMediaTrackID, int64 p_vCutPos, list<EMCommand*> p_oUndoCommands) :
m_oCutEvents(p_oCutEvents),
m_opClipContainer(p_opClipContainer),
m_opTrackView(p_opTrackView),
m_oUndoCommands(p_oUndoCommands),
m_vCutPos(p_vCutPos),
m_vMediaTrackID(p_vMediaTrackID)
{
}

CommandCutMIDIEvents::~CommandCutMIDIEvents()
{
}

EMCommand* CommandCutMIDIEvents::CloneAndClear()
{
	CommandCutMIDIEvents* opCommand = EM_new CommandCutMIDIEvents(m_opTrackView, m_opClipContainer, m_oCutEvents, m_vMediaTrackID, m_vCutPos, m_oUndoCommands);
	m_oCutEvents.clear();
	m_oUndoCommands.clear();
	return opCommand;
}

// FIXME: This command can only handle one MIDI event right now
void* CommandCutMIDIEvents::ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree)
{
	m_oCutEvents = *(static_cast<list<EMMediaMIDIEvent*>*>(p_opParameterOne));
	m_vMediaTrackID = *(static_cast<int32*>(p_opParameterTwo));
	m_vCutPos = *(static_cast<int64*>(p_opParameterThree));

	EMCommand* opUndoCommand;
	
	m_opClipContainer = static_cast<MIDIGClipContainer*>(EMGClipContainer::GetClipContainerWithSelection());

	list<EMMediaMIDIEvent*> oEventsToClone = m_oCutEvents;
	oEventsToClone.push_back(m_oCutEvents.front() -> m_opNoteOff);
	int64 vpCloneData[4];
	vpCloneData[0] = m_vMediaTrackID;
	vpCloneData[1] = -1;
	vpCloneData[2] = 0;
	vpCloneData[3] = 0;
	// Assume that the first new event i a note on event
	EMMediaMIDIEvent* opNewEvent = (static_cast<list<EMMediaMIDIEvent*>*>(EMCommandRepository::Instance() -> ExecuteCommandNoUndo(COMMAND_CLONE_MIDI_EVENTS, vpCloneData, m_opClipContainer, &oEventsToClone))) -> front();
	opUndoCommand = EMCommandRepository::Instance() -> GetUndoClone(COMMAND_CLONE_MIDI_EVENTS);
	m_oUndoCommands.push_back(opUndoCommand);

	int64 vPosition;
	int64 vLength;

	vPosition = *(m_oCutEvents.front() -> m_vpStart);
	vLength = m_vCutPos - vPosition;
	EMCommandRepository::Instance() -> ExecuteCommandNoUndo(MEDIA_COMMAND_RESIZE_MIDI_EVENT, m_oCutEvents.front(), &vPosition, &vLength);
	opUndoCommand = EMCommandRepository::Instance() -> GetUndoClone(MEDIA_COMMAND_RESIZE_MIDI_EVENT);
	m_oUndoCommands.push_back(opUndoCommand);

	vPosition = m_vCutPos;
	vLength = *(opNewEvent -> m_opNoteOff -> m_vpStart) - m_vCutPos + 1;
	EMCommandRepository::Instance() -> ExecuteCommandNoUndo(MEDIA_COMMAND_RESIZE_MIDI_EVENT, opNewEvent, &vPosition, &vLength);
	opUndoCommand = EMCommandRepository::Instance() -> GetUndoClone(MEDIA_COMMAND_RESIZE_MIDI_EVENT);
	m_oUndoCommands.push_back(opUndoCommand);

	m_opClipContainer -> UpdateEventsInClips();
	m_opTrackView -> GetView() -> Invalidate(m_opClipContainer -> Frame() & m_opTrackView -> GetView() -> Bounds());

	char vOutput[80];
	if(m_oCutEvents.size() == 1)
		sprintf(vOutput, "Event cut");
	else
		sprintf(vOutput, "%ld events cut", m_oCutEvents.size());
	TrackWindowStatusBar::Instance() -> WriteMessage(string(vOutput));

	return opNewEvent;
}

const char* CommandCutMIDIEvents::GetShortDescription()
{
	return "Cut MIDI Events";
}

bool CommandCutMIDIEvents::IsUndoable()
{
	return true;
}

bool CommandCutMIDIEvents::RequiresParameters()
{
	return true;
}

void CommandCutMIDIEvents::UndoE()
{
	if(m_opTrackView -> GetView() -> LockWindow())
	{
		EMCommand* opCommand;
		while(m_oUndoCommands.size() > 0)
		{
			opCommand = m_oUndoCommands.back();
			m_oUndoCommands.pop_back();
			opCommand -> UndoE();
			delete opCommand;
		}

		m_opTrackView -> GetView() -> Invalidate(m_opClipContainer -> Frame() & m_opTrackView -> GetView() -> Bounds());
	
		m_opTrackView -> GetView() -> UnlockWindow();
	}
}
