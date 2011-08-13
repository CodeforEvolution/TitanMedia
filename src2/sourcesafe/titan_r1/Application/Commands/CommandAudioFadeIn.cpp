#include "CommandAudioFadeIn.h"

#include "CommandIDs.h"
#include "EMView.h"
#include "TrackView.h"

CommandAudioFadeIn::CommandAudioFadeIn(TrackView* p_opTrackView) :
DestructiveEditBase(p_opTrackView)
{
}

CommandAudioFadeIn::CommandAudioFadeIn(TrackView* p_opTrackView, list<AudioGClip*> p_oAffectedClips, list<AudioGClip*> p_oNewClips, list<EMCommand*> p_oUndoCommands) :
DestructiveEditBase(p_opTrackView, p_oAffectedClips, p_oNewClips, p_oUndoCommands)
{
}

EMCommand* CommandAudioFadeIn::CloneAndClear()
{
	EMCommand* opCommand = EM_new CommandAudioFadeIn(m_opTrackView, m_oAffectedClips, m_oNewClips, m_oUndoCommands);
	m_oAffectedClips.clear();
	m_oNewClips.clear();
	m_oUndoCommands.clear();

	return opCommand;
}

void CommandAudioFadeIn::ExecuteE()
{
	DestructiveEdit(MEDIA_COMMAND_DESTRUCTIVE_FADE_IN);
}

const char* CommandAudioFadeIn::GetShortDescription()
{
	return "Audio Fade In";
}

bool CommandAudioFadeIn::IsUndoable()
{
	return true;
}

bool CommandAudioFadeIn::RequiresParameters()
{
	return false;
}

void CommandAudioFadeIn::UndoE()
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

		m_opTrackView -> GetView() -> Invalidate(m_opTrackView -> GetView() -> Bounds());

		m_opTrackView -> GetView() -> UnlockWindow();
	}
}
