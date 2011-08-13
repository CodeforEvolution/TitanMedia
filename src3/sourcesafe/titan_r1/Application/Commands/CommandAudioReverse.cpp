#include "CommandAudioReverse.h"

#include "CommandIDs.h"
#include "EMView.h"
#include "TrackView.h"

CommandAudioReverse::CommandAudioReverse(TrackView* p_opTrackView) :
DestructiveEditBase(p_opTrackView)
{
}

CommandAudioReverse::CommandAudioReverse(TrackView* p_opTrackView, list<AudioGClip*> p_oAffectedClips, list<AudioGClip*> p_oNewClips, list<EMCommand*> p_oUndoCommands) :
DestructiveEditBase(p_opTrackView, p_oAffectedClips, p_oNewClips, p_oUndoCommands)
{
}

EMCommand* CommandAudioReverse::CloneAndClear()
{
	EMCommand* opCommand = EM_new CommandAudioReverse(m_opTrackView, m_oAffectedClips, m_oNewClips, m_oUndoCommands);
	m_oAffectedClips.clear();
	m_oNewClips.clear();
	m_oUndoCommands.clear();

	return opCommand;
}

void CommandAudioReverse::ExecuteE()
{
	DestructiveEdit(MEDIA_COMMAND_DESTRUCTIVE_REVERSE);
}

const char* CommandAudioReverse::GetShortDescription()
{
	return "Audio Reverse";
}

bool CommandAudioReverse::IsUndoable()
{
	return true;
}

bool CommandAudioReverse::RequiresParameters()
{
	return false;
}

void CommandAudioReverse::UndoE()
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
