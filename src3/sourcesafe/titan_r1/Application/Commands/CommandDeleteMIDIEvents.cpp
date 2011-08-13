#include "CommandDeleteMIDIEvents.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGClipRepository.h"
#include "EMMediaMIDIEvent.h"
#include "EMView.h"
#include "MIDIGClipContainer.h"
#include "TrackView.h"
#include "TrackWindowStatusBar.h"

#include <string>

CommandDeleteMIDIEvents::CommandDeleteMIDIEvents(TrackView* p_opTrackView) :
m_opTrackView(p_opTrackView)
{
}

CommandDeleteMIDIEvents::CommandDeleteMIDIEvents(TrackView* p_opTrackView, MIDIGClipContainer* p_opClipContainer, list<EMCommand*> p_oUndoCommands, int32 p_vMediaTrackID) :
m_oUndoCommands(p_oUndoCommands),
m_opClipContainer(p_opClipContainer),
m_opTrackView(p_opTrackView),
m_vMediaTrackID(p_vMediaTrackID)
{
}

CommandDeleteMIDIEvents::~CommandDeleteMIDIEvents()
{
}

EMCommand* CommandDeleteMIDIEvents::CloneAndClear()
{
	CommandDeleteMIDIEvents* opCommand = EM_new CommandDeleteMIDIEvents(m_opTrackView, m_opClipContainer, m_oUndoCommands, m_vMediaTrackID);
	m_oUndoCommands.clear();
	return opCommand;
}

void* CommandDeleteMIDIEvents::ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree)
{
	list<EMMediaMIDIEvent*> oDeletedEvents = *(static_cast<list<EMMediaMIDIEvent*>*>(p_opParameterOne));
	m_vMediaTrackID = *(static_cast<int32*>(p_opParameterTwo));

	list<int32>* opDeletedMediaClips = static_cast<list<int32>*>(EMCommandRepository::Instance() -> ExecuteCommandNoUndo(MEDIA_COMMAND_DELETE_MIDI_EVENTS, &oDeletedEvents));
	m_oUndoCommands.push_back(EMCommandRepository::Instance() -> GetUndoClone(MEDIA_COMMAND_DELETE_MIDI_EVENTS));

	m_opClipContainer = static_cast<MIDIGClipContainer*>(EMGClipContainer::GetClipContainerWithSelection());
	m_opClipContainer -> DeselectAll(m_opClipContainer);

	if(opDeletedMediaClips -> size() > 0)
	{
		list<EMGClip*> oClipsToDelete;
		list<int32>::iterator oIterator = opDeletedMediaClips -> begin();
		while(oIterator != opDeletedMediaClips -> end())
		{
			oClipsToDelete.push_back(EMGClipRepository::Instance() -> GetClip(*oIterator));
			oIterator++;
		}
eo << "x1" << ef;
		EMCommandRepository::Instance() -> ExecuteCommandNoUndo(COMMAND_DELETE_CLIPS_FROM_LIST, &oClipsToDelete);
eo << "x2" << ef;
		m_oUndoCommands.push_back(EMCommandRepository::Instance() -> GetUndoClone(COMMAND_DELETE_CLIPS_FROM_LIST));
eo << "x3" << ef;
	}

	//m_opClipContainer -> CheckForDeletedClips();
	m_opClipContainer -> UpdateEventsInClips();

	m_opTrackView -> GetView() -> Invalidate(m_opClipContainer -> Frame() & m_opTrackView -> GetView() -> Bounds());

	char vOutput[80];
	if(oDeletedEvents.size() == 1)
		sprintf(vOutput, "Event deleted");
	else
		sprintf(vOutput, "%ld events deleted", oDeletedEvents.size());
	TrackWindowStatusBar::Instance() -> WriteMessage(string(vOutput));
eo << "x4" << ef;

	return NULL;
}

const char* CommandDeleteMIDIEvents::GetShortDescription()
{
	return "Delete MIDI Events";
}

bool CommandDeleteMIDIEvents::IsUndoable()
{
	return true;
}

bool CommandDeleteMIDIEvents::RequiresParameters()
{
	return true;
}

void CommandDeleteMIDIEvents::UndoE()
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

		m_opClipContainer -> UpdateEventsInClips();
	
		m_opTrackView -> GetView() -> Invalidate(m_opClipContainer -> Frame() & m_opTrackView -> GetView() -> Bounds());
	
		m_opTrackView -> GetView() -> UnlockWindow();
	}
}
