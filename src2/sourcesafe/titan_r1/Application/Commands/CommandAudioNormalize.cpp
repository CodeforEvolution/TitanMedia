#include "CommandAudioNormalize.h"

#include "CommandIDs.h"
#include "EMView.h"
#include "TrackView.h"

CommandAudioNormalize::CommandAudioNormalize(TrackView* p_opTrackView) :
DestructiveEditBase(p_opTrackView)
{
}

CommandAudioNormalize::CommandAudioNormalize(TrackView* p_opTrackView, list<AudioGClip*> p_oAffectedClips, list<AudioGClip*> p_oNewClips, list<EMCommand*> p_oUndoCommands) :
DestructiveEditBase(p_opTrackView, p_oAffectedClips, p_oNewClips, p_oUndoCommands)
{
}

EMCommand* CommandAudioNormalize::CloneAndClear()
{
	EMCommand* opCommand = EM_new CommandAudioNormalize(m_opTrackView, m_oAffectedClips, m_oNewClips, m_oUndoCommands);
	m_oAffectedClips.clear();
	m_oNewClips.clear();
	m_oUndoCommands.clear();

	return opCommand;
}

void CommandAudioNormalize::DialogCallback(int32 p_vDialogParameter)
{
	DestructiveEdit(MEDIA_COMMAND_DESTRUCTIVE_NORMALIZE, -1, true, p_vDialogParameter);
}

void CommandAudioNormalize::ExecuteE()
{
	EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DISPLAY_NORMALIZE_DIALOG, this);
}

const char* CommandAudioNormalize::GetShortDescription()
{
	return "Audio Normalize";
}

bool CommandAudioNormalize::IsUndoable()
{
	return true;
}

bool CommandAudioNormalize::RequiresParameters()
{
	return false;
}

void CommandAudioNormalize::UndoE()
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
