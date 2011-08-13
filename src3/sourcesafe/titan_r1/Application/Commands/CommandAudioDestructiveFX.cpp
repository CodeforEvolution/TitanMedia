#include "CommandAudioDestructiveFX.h"

#include "CommandIDs.h"
#include "EMView.h"
#include "TrackView.h"

CommandAudioDestructiveFX::CommandAudioDestructiveFX(TrackView* p_opTrackView) :
DestructiveEditBase(p_opTrackView)
{
}

CommandAudioDestructiveFX::CommandAudioDestructiveFX(TrackView* p_opTrackView, list<AudioGClip*> p_oAffectedClips, list<AudioGClip*> p_oNewClips, list<EMCommand*> p_oUndoCommands) :
DestructiveEditBase(p_opTrackView, p_oAffectedClips, p_oNewClips, p_oUndoCommands)
{
}

EMCommand* CommandAudioDestructiveFX::CloneAndClear()
{
	EMCommand* opCommand = EM_new CommandAudioDestructiveFX(m_opTrackView, m_oAffectedClips, m_oNewClips, m_oUndoCommands);
	m_oAffectedClips.clear();
	m_oNewClips.clear();
	m_oUndoCommands.clear();

	return opCommand;
}

void* CommandAudioDestructiveFX::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{
	int32 vEffectEntryID = *(static_cast<int32*>(p_upParameterOne));
	DestructiveEdit(MEDIA_COMMAND_DESTRUCTIVE_EDIT, vEffectEntryID, true);
	return NULL;
}

const char* CommandAudioDestructiveFX::GetShortDescription()
{
	return "Audio Plugin Effect";
}

bool CommandAudioDestructiveFX::IsUndoable()
{
	return true;
}

bool CommandAudioDestructiveFX::RequiresParameters()
{
	return true;
}

void CommandAudioDestructiveFX::UndoE()
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
