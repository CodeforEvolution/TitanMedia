#include "CommandDeleteClipsEventsNodes.h"

#include "AudioGClipContainer.h"
#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGRubberBand.h"
#include "EMGTrack.h"
#include "EMGTrackInfo.h"
#include "EMGUIGlobals.h"
#include "EMMediaMIDIEvent.h"
#include "EMMIDIGlobals.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMView.h"
#include "MIDIGClipContainer.h"
#include "SelectionButton.h"
#include "TrackView.h"

#include <list>

CommandDeleteClipsEventsNodes::CommandDeleteClipsEventsNodes(TrackView* p_opTrackView) :
m_opTrackView(p_opTrackView)
{
}

CommandDeleteClipsEventsNodes::~CommandDeleteClipsEventsNodes()
{
}

void CommandDeleteClipsEventsNodes::ExecuteE()
{
	EMEditMode eEditMode = static_cast<EMEditMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EDIT_MODE))));

	if(eEditMode == EM_FX_EDIT_MODE)
		return;

	if(m_opTrackView -> GetView() -> LockWindow())
	{		
		if(eEditMode == EM_TRACK_EDIT_MODE)
			EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DELETE_CLIPS);

		else
		{
			EMGClipContainer* opContainer = EMGClipContainer::GetClipContainerWithSelection();

			if(opContainer == NULL)
			{
				m_opTrackView -> GetView() -> UnlockWindow();
				return;
			}

			if(dynamic_cast<MIDIGClipContainer*>(opContainer))
			{
				MIDIGClipContainer* opMIDIContainer;
				opMIDIContainer = static_cast<MIDIGClipContainer*>(opContainer);

				ClipEditDisplayMode eDisplayMode = opMIDIContainer -> GetDisplayMode();

				if(eDisplayMode == CE_DISPLAY_MODE_DEFAULT) // If MIDI events are affected
				{
					list<EMMediaMIDIEvent*>* opEvents = opMIDIContainer -> GetSelectedMIDIEvents();

					// Include note off events if they aren't already included
					list<EMMediaMIDIEvent*> oEventsToBeDeleted;
					list<EMMediaMIDIEvent*>::iterator oIterator = opEvents -> begin();
					while(oIterator != opEvents -> end())
					{
						oEventsToBeDeleted.push_back(*oIterator);
						if((*oIterator) -> m_vBaseType == EM_MIDI_EVENT_TYPE_NOTE_ON)
							oEventsToBeDeleted.push_back((*oIterator) -> m_opNoteOff);

						oIterator++;
					}

					int32 vMediaTrackID = opContainer -> GetTrack() -> GetTrackInfo() -> GetMediaTrackID();
					EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DELETE_MIDI_EVENTS, &oEventsToBeDeleted, &vMediaTrackID);
				}

				else if(eDisplayMode != CE_DISPLAY_MODE_MIDI_VELOCITY) // Pitch bend events or other track-based rubberband events
				{
					list<EMGRubberBandNode*>* opNodes = opMIDIContainer -> GetRubberband() -> GetSelectedNodes();
					if(opNodes -> size() > 0)
						opMIDIContainer -> GetRubberband() -> DeleteNodes(opNodes, false);
				}

				m_opTrackView -> GetView() -> Invalidate(opMIDIContainer -> Frame() & m_opTrackView -> GetView() -> Bounds());
			}

			else if(dynamic_cast<AudioGClipContainer*>(opContainer))
			{
				AudioGClipContainer* opAudioContainer;
				opAudioContainer = static_cast<AudioGClipContainer*>(opContainer);

				ClipEditDisplayMode eDisplayMode = opAudioContainer -> GetDisplayMode();

				if(eDisplayMode == CE_DISPLAY_MODE_DEFAULT) // If parts of/whole clips are affected
				{
					// FIXME: make it possible to resize events here so that the audio selection becomes free from clips
				}

				else // Track-based rubberband events (currently volume and pan)
				{
					list<EMGRubberBandNode*>* opNodes = opAudioContainer -> GetRubberband() -> GetSelectedNodes();
					if(opNodes -> size() > 0)
						opAudioContainer -> GetRubberband() -> DeleteNodes(opNodes, false);
				}

				m_opTrackView -> GetView() -> Invalidate(opAudioContainer -> Frame() & m_opTrackView -> GetView() -> Bounds());
			}
		}

		m_opTrackView -> GetView() -> UnlockWindow();
	}
}

const char* CommandDeleteClipsEventsNodes::GetShortDescription()
{
	return "Delete clips/events/nodes";
}

bool CommandDeleteClipsEventsNodes::IsUndoable()
{
	return false;
}

bool CommandDeleteClipsEventsNodes::RequiresParameters()
{
	return false;
}

