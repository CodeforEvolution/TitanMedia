#include "CommandPrepareRecording.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMView.h"
#include "TrackWindowToolBar.h"
// FIXME: Perhaps this command should be renamed to CommandPrepareAndStartRecording
CommandPrepareRecording::CommandPrepareRecording(TrackWindowToolBar* p_opToolBar) :
m_opToolBar(p_opToolBar)
{
}

void CommandPrepareRecording::ExecuteE()
{
	if(EMCommandRepository::Instance() -> TriggeredByBinding())
	{
		if(m_opToolBar -> GetView() -> LockWindow())
		{
			m_opToolBar -> SetRecord();
			m_opToolBar -> SetPlay();
			m_opToolBar -> GetView() -> UnlockWindow();
		}
	}
	else
	{
		if(m_opToolBar -> GetView() -> LockWindow())
		{
			m_opToolBar -> SetPlay();
			m_opToolBar -> GetView() -> UnlockWindow();
		}
	}
	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_PREPARE_RECORDING);
	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_START_PLAYBACK);
}

bool CommandPrepareRecording::IsUndoable()
{
	return false;
}

bool CommandPrepareRecording::RequiresParameters()
{
	return false;
}


