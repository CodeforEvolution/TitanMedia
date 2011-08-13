#include "CommandAudioSilence.h"

#include "CommandIDs.h"
#include "EMView.h"
#include "TrackView.h"

CommandAudioSilence::CommandAudioSilence(TrackView* p_opTrackView) :
DestructiveEditBase(p_opTrackView)
{
}

CommandAudioSilence::CommandAudioSilence(TrackView* p_opTrackView, list<AudioGClip*> p_oAffectedClips, list<AudioGClip*> p_oNewClips, list<EMCommand*> p_oUndoCommands) :
DestructiveEditBase(p_opTrackView, p_oAffectedClips, p_oNewClips, p_oUndoCommands)
{
}

EMCommand* CommandAudioSilence::CloneAndClear()
{
	EMCommand* opCommand = EM_new CommandAudioSilence(m_opTrackView, m_oAffectedClips, m_oNewClips, m_oUndoCommands);
	m_oAffectedClips.clear();
	m_oNewClips.clear();
	m_oUndoCommands.clear();

	return opCommand;
}

void CommandAudioSilence::ExecuteE()
{
	DestructiveEdit(MEDIA_COMMAND_DESTRUCTIVE_SILENCE);
}

const char* CommandAudioSilence::GetShortDescription()
{
	return "Audio Silence";
}

bool CommandAudioSilence::IsUndoable()
{
	return true;
}

bool CommandAudioSilence::RequiresParameters()
{
	return false;
}

void CommandAudioSilence::UndoE()
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
