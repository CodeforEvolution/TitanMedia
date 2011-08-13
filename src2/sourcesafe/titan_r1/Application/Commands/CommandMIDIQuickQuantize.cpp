#include "CommandMIDIQuickQuantize.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGClip.h"
#include "EMGClipContainer.h"
#include "EMGClipRepository.h"
#include "EMMediaMIDIEvent.h"
#include "EMMIDIGlobals.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "MIDIGClip.h"
#include "MIDIGClipContainer.h"
#include "SelectionButton.h"
#include "TrackView.h"
#include "TrackWindow.h"

CommandMIDIQuickQuantize::CommandMIDIQuickQuantize(TrackWindow* p_opTrackWindow) :
m_opTrackWindow(p_opTrackWindow)
{
}

CommandMIDIQuickQuantize::CommandMIDIQuickQuantize(TrackWindow* p_opTrackWindow, list<EMGClip*> p_oAffectedClips, EMCommand* p_opMediaUndoCommand) :
m_opTrackWindow(p_opTrackWindow),
m_oAffectedClips(p_oAffectedClips),
m_opMediaUndoCommand(p_opMediaUndoCommand)
{
}

EMCommand* CommandMIDIQuickQuantize::CloneAndClear()
{
	EMCommand* opCommand = EM_new CommandMIDIQuickQuantize(m_opTrackWindow, m_oAffectedClips, m_opMediaUndoCommand);
	m_oAffectedClips.clear();
	return opCommand;
}

void CommandMIDIQuickQuantize::ExecuteE()
{
	EMEditMode eEditMode = static_cast<EMEditMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EDIT_MODE))));

	if(eEditMode == EM_FX_EDIT_MODE)
		return;

	if(m_opTrackWindow -> GetTrackInfo() -> GetView() -> LockWindow())
	{
		if(eEditMode == EM_TRACK_EDIT_MODE)
		{
			// Build a list of all affected MIDI clips
			list<EMGClip*>::iterator oClipIterator = EMGClipRepository::Instance() -> GetSelectedClips() -> begin();
			while(oClipIterator != EMGClipRepository::Instance() -> GetSelectedClips() -> end())
			{
				if((*oClipIterator) -> GetType() == EM_TYPE_MIDI)
					m_oAffectedClips.push_back(*oClipIterator);

				oClipIterator++;
			}

			// Build a list of all affected MIDI events
			list<EMMediaMIDIEvent*> oClipEvents;
			list<EMMediaMIDIEvent*> oTotalEvents;
			list<EMMediaMIDIEvent*>::iterator oEventIterator;
			oClipIterator = m_oAffectedClips.begin();
			while(oClipIterator != m_oAffectedClips.end())
			{
				oClipEvents = *(static_cast<MIDIGClip*>(*oClipIterator) -> GetMIDIEvents());
				oEventIterator = oClipEvents.begin();
				while(oEventIterator != oClipEvents.end())
				{
					if((*oEventIterator) -> m_vBaseType == EM_MIDI_EVENT_TYPE_NOTE_ON)// || (*oEventIterator) -> m_vBaseType == EM_MIDI_EVENT_TYPE_NOTE_OFF)
						oTotalEvents.push_back((*oEventIterator) -> m_opNoteOff);

					oEventIterator++;
				}

				oClipIterator++;
			}

			// Do the actual quantizing
			EMCommandRepository::Instance() -> ExecuteCommandNoUndo(MEDIA_COMMAND_QUANTIZE_MIDI_EVENTS, &oTotalEvents);
			m_opMediaUndoCommand = EMCommandRepository::Instance() -> GetUndoClone(MEDIA_COMMAND_QUANTIZE_MIDI_EVENTS);

			// Update the clips
			oClipIterator = m_oAffectedClips.begin();
			while(oClipIterator != m_oAffectedClips.end())
			{
				static_cast<MIDIGClip*>(*oClipIterator) -> UpdateContent();
				(*oClipIterator) -> UpdateFrame();
				oClipIterator++;
			}

			// Update the selection rects
			EMGClipContainer* opContainer = EMGClipContainer::GetClipContainerWithSelection();
			if(opContainer != NULL)
				if(dynamic_cast<MIDIGClipContainer*>(opContainer))
					static_cast<MIDIGClipContainer*>(opContainer) -> UpdateSelection();
			EMGClipRepository::Instance() -> CalculateSelectedFrame();
				
			m_opTrackWindow -> GetTrackView() -> GetView() -> Invalidate(EMGClipRepository::Instance() -> SelectedFrame());
		}

		else
		{
			EMGClipContainer* opContainer = EMGClipContainer::GetClipContainerWithSelection();

			if(opContainer == NULL)
			{
				m_opTrackWindow -> GetTrackView() -> GetView() -> UnlockWindow();
				return;
			}

			if(dynamic_cast<MIDIGClipContainer*>(opContainer))
			{
				MIDIGClipContainer* opMIDIContainer;
				opMIDIContainer = static_cast<MIDIGClipContainer*>(opContainer);

				ClipEditDisplayMode eDisplayMode = opMIDIContainer -> GetDisplayMode();

				if(eDisplayMode == CE_DISPLAY_MODE_DEFAULT) // If MIDI events are affected
				{
					// Affected MIDI clips are all clips in the clip container (quick 'n dirty solution)
					m_oAffectedClips = *(opMIDIContainer -> GetClips());

					list<EMMediaMIDIEvent*>* opEvents = opMIDIContainer -> GetSelectedMIDIEvents();

					// Include note off events if they aren't already included
					list<EMMediaMIDIEvent*> oTotalEvents;
					list<EMMediaMIDIEvent*>::iterator oIterator = opEvents -> begin();
					while(oIterator != opEvents -> end())
					{
						oTotalEvents.push_back(*oIterator);
						//if((*oIterator) -> m_vBaseType == EM_MIDI_EVENT_TYPE_NOTE_ON)
						//	oTotalEvents.push_back((*oIterator) -> m_opNoteOff);

						oIterator++;
					}

					// Do the actual quantizing
					EMCommandRepository::Instance() -> ExecuteCommandNoUndo(MEDIA_COMMAND_QUANTIZE_MIDI_EVENTS, &oTotalEvents);
					m_opMediaUndoCommand = EMCommandRepository::Instance() -> GetUndoClone(MEDIA_COMMAND_QUANTIZE_MIDI_EVENTS);

					// Update the clips
					opMIDIContainer -> UpdateEventsInClips();
					
					// Update the selection rects
					opMIDIContainer -> UpdateSelection();
					EMGClipRepository::Instance() -> CalculateSelectedFrame();

					m_opTrackWindow -> GetTrackView() -> GetView() -> Invalidate(opMIDIContainer -> Frame() & m_opTrackWindow -> GetTrackView() -> GetView() -> Bounds());
				}

				else if(eDisplayMode != CE_DISPLAY_MODE_MIDI_VELOCITY) // Pitch bend events or other track-based rubberband events
				{
					m_opTrackWindow -> GetTrackView() -> GetView() -> UnlockWindow();
					return;
				}
			}
		}

		m_opTrackWindow -> GetTrackInfo() -> GetView() -> UnlockWindow();
	}
}

bool CommandMIDIQuickQuantize::IsUndoable()
{
	return true;
}

bool CommandMIDIQuickQuantize::RequiresParameters()
{
	return false;
}

void CommandMIDIQuickQuantize::UndoE()
{
	if(m_opTrackWindow -> GetTrackInfo() -> GetView() -> LockWindow())
	{
		m_opMediaUndoCommand -> UndoE();

		// Update the clips
		list<EMGClip*>::iterator oClipIterator = m_oAffectedClips.begin();
		while(oClipIterator != m_oAffectedClips.end())
		{
			static_cast<MIDIGClip*>(*oClipIterator) -> UpdateContent();
			(*oClipIterator) -> UpdateFrame();
			oClipIterator++;
		}

		m_opTrackWindow -> GetTrackView() -> GetView() -> Invalidate(m_opTrackWindow -> GetTrackView() -> GetView() -> Bounds());

		m_opTrackWindow -> GetTrackInfo() -> GetView() -> UnlockWindow();
	}
}

