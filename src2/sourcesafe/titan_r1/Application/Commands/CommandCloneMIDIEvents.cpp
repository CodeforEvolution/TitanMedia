#include "CommandCloneMIDIEvents.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMMediaMIDIEvent.h"
#include "EMMIDIGlobals.h"
#include "EMView.h"
#include "MIDIGClipContainer.h"
#include "TrackView.h"
#include "TrackWindowStatusBar.h"

#include <string>

CommandCloneMIDIEvents::CommandCloneMIDIEvents(TrackView* p_opTrackView) :
m_opTrackView(p_opTrackView)
{
}

CommandCloneMIDIEvents::CommandCloneMIDIEvents(TrackView* p_opTrackView, MIDIGClipContainer* p_opClipContainer, EMCommand* p_opMediaUndoCommand) :
m_opMediaUndoCommand(p_opMediaUndoCommand),
m_opClipContainer(p_opClipContainer),
m_opTrackView(p_opTrackView)
{
}

CommandCloneMIDIEvents::~CommandCloneMIDIEvents()
{
}

EMCommand* CommandCloneMIDIEvents::CloneAndClear()
{
	return EM_new CommandCloneMIDIEvents(m_opTrackView, m_opClipContainer, m_opMediaUndoCommand);
}

// p_vpData contains:
//		media track id
//		media clip id
//		key offset
//		position offset
void* CommandCloneMIDIEvents::ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree)
{
	int64* vpData = static_cast<int64*>(p_opParameterOne);
	m_opClipContainer = static_cast<MIDIGClipContainer*>(p_opParameterTwo);
	list<EMMediaMIDIEvent*> oEventsToBeCloned = *(static_cast<list<EMMediaMIDIEvent*>*>(p_opParameterThree));

	int64* vpEventData = EM_new int64[oEventsToBeCloned.size() * 5];
	int vEventDataIndex(0);
	list<EMMediaMIDIEvent*>::iterator oIterator = oEventsToBeCloned.begin();
	while(oIterator != oEventsToBeCloned.end())
	{
		vpEventData[vEventDataIndex] = (*oIterator) -> m_vBaseType;
		vEventDataIndex++;
		if((*oIterator) -> m_vBaseType == EM_MIDI_EVENT_TYPE_NOTE_ON || (*oIterator) -> m_vBaseType == EM_MIDI_EVENT_TYPE_NOTE_OFF)
			vpEventData[vEventDataIndex] = (*oIterator) -> m_vData1 + vpData[2];
		else
			vpEventData[vEventDataIndex] = (*oIterator) -> m_vData1;
		vEventDataIndex++;
		vpEventData[vEventDataIndex] = (*oIterator) -> m_vData2;
		vEventDataIndex++;
		vpEventData[vEventDataIndex] = (*oIterator) -> m_vDataLength;
		vEventDataIndex++;
		vpEventData[vEventDataIndex] = *((*oIterator) -> m_vpStart) + vpData[3];
		vEventDataIndex++;

		oIterator++;
	}

	int32 vpParameters[3];
	vpParameters[0] = vpData[0];
	vpParameters[1] = vpData[1];
	vpParameters[2] = oEventsToBeCloned.size();

	m_oClonedEvents.clear();
	m_oClonedEvents = *(static_cast<list<EMMediaMIDIEvent*>*>(EMCommandRepository::Instance() -> ExecuteCommandNoUndo(MEDIA_COMMAND_CREATE_MIDI_EVENT, vpParameters, vpEventData)));
	m_opMediaUndoCommand = EMCommandRepository::Instance() -> GetUndoClone(MEDIA_COMMAND_CREATE_MIDI_EVENT);

	if(m_opClipContainer != NULL)
		if(m_opTrackView -> GetView() -> LockWindow())
		{
			m_opClipContainer -> UpdateEventsInClips();
			m_opTrackView -> GetView() -> Invalidate(m_opClipContainer -> Frame());
			m_opTrackView -> GetView() -> UnlockWindow();
		}

	char vOutput[80];
	if(oEventsToBeCloned.size() == 1)
		sprintf(vOutput, "Event cloned, Offset: %ld, %lld", vpData[2], vpData[3]);
	else
		sprintf(vOutput, "%ld events cloned, Offset: %ld, %lld", oEventsToBeCloned.size(), vpData[2], vpData[3]);
	TrackWindowStatusBar::Instance() -> WriteMessage(string(vOutput));

	delete [] vpEventData;
	return &m_oClonedEvents;
}

const char* CommandCloneMIDIEvents::GetShortDescription()
{
	return "Move MIDI Events";
}

bool CommandCloneMIDIEvents::IsUndoable()
{
	return true;
}

bool CommandCloneMIDIEvents::RequiresParameters()
{
	return true;
}

void CommandCloneMIDIEvents::UndoE()
{
	m_opMediaUndoCommand -> UndoE();
	delete m_opMediaUndoCommand;

	if(m_opClipContainer != NULL)
		if(m_opTrackView -> GetView() -> LockWindow())
		{
			m_opClipContainer -> UpdateEventsInClips();
			m_opTrackView -> GetView() -> Invalidate(m_opClipContainer -> Frame());
			m_opTrackView -> GetView() -> UnlockWindow();
		}
}
