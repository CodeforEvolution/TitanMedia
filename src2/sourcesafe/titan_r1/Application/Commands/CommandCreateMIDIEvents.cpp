#include "CommandCreateMIDIEvents.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGClip.h"
//#include "EMGTrackInfo.h"
#include "EMView.h"
#include "MIDIGClipContainer.h"
#include "TrackView.h"
#include "TrackWindowStatusBar.h"

#include <string>

CommandCreateMIDIEvents::CommandCreateMIDIEvents(TrackView* p_opTrackView) :
m_opTrackView(p_opTrackView)
{
}

CommandCreateMIDIEvents::CommandCreateMIDIEvents(TrackView* p_opTrackView, MIDIGClipContainer* p_opClipContainer, list<EMCommand*> p_oUndoCommands) :
m_oUndoCommands(p_oUndoCommands),
m_opClipContainer(p_opClipContainer),
m_opTrackView(p_opTrackView)
{
}

CommandCreateMIDIEvents::~CommandCreateMIDIEvents()
{
}

EMCommand* CommandCreateMIDIEvents::CloneAndClear()
{
	EMCommand* opCommand = EM_new CommandCreateMIDIEvents(m_opTrackView, m_opClipContainer, m_oUndoCommands);
	m_oUndoCommands.clear();

	return opCommand;
}

// p_vpParameters (int32) contains:
// 1: media track id
// 2: media clip id (-1 if the media engine should assign it to a clip)
// 3: number of events to be created
// p_vpEventData (int64) should contain multiples of 5 bytes (stored in an array of int64:s) in the following format and order:
// 1: type (e.g. EM_MIDI_EVENT_TYPE_NOTE_ON)
// 2: data 1 (e.g. key)
// 3: data 2 (e.g. velocity)
// 4: data length (e.g. 3)
// 5: position
//
// If parameter 3 is NULL, a display refresh (and other updates) should be made somewhere else
void* CommandCreateMIDIEvents::ExecuteE(void* p_vpParameters, void* p_vpEventData, void* p_opClipContainer)
{
	m_opClipContainer = static_cast<MIDIGClipContainer*>(p_opClipContainer);

	EMCommand* opUndoCommand = NULL;
	m_opEvents = static_cast<list<EMMediaMIDIEvent*>*>(EMCommandRepository::Instance() -> ExecuteCommandNoUndo(MEDIA_COMMAND_CREATE_MIDI_EVENT, p_vpParameters, p_vpEventData, &opUndoCommand));
	if(opUndoCommand != NULL)
		m_oUndoCommands.push_back(opUndoCommand);
	m_oUndoCommands.push_back(EMCommandRepository::Instance() -> GetUndoClone(MEDIA_COMMAND_CREATE_MIDI_EVENT));

	if(m_opClipContainer != NULL)
		if(m_opTrackView -> GetView() -> LockWindow())
		{
			m_opClipContainer -> UpdateEventsInClips();
			m_opTrackView -> GetView() -> Invalidate(m_opClipContainer -> Frame());
			m_opTrackView -> GetView() -> UnlockWindow();
		}

	int vNrEvents = (static_cast<int32*>(p_vpParameters))[2];
	char vOutput[80];
	sprintf(vOutput, "%ld MIDI event(s) created", vNrEvents);
	TrackWindowStatusBar::Instance() -> WriteMessage(string(vOutput));

	return m_opEvents;
}

const char* CommandCreateMIDIEvents::GetShortDescription()
{
	return "Create MIDI Events";
}

bool CommandCreateMIDIEvents::IsUndoable()
{
	return true;
}

bool CommandCreateMIDIEvents::RequiresParameters()
{
	return true;
}

void CommandCreateMIDIEvents::UndoE()
{
	EMCommand* opCommand;
	while(m_oUndoCommands.size() > 0)
	{
		opCommand = m_oUndoCommands.back();
		m_oUndoCommands.pop_back();
		opCommand -> UndoE();
		delete opCommand;
	}

	if(m_opClipContainer != NULL)
		if(m_opTrackView -> GetView() -> LockWindow())
		{
			m_opClipContainer -> UpdateEventsInClips();
			m_opTrackView -> GetView() -> Invalidate(m_opClipContainer -> Frame() & m_opTrackView -> GetView() -> Bounds());
			m_opClipContainer -> DeselectAll(m_opClipContainer);

			m_opTrackView -> GetView() -> UnlockWindow();
		}
}
