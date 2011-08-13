#include "CommandTogglePlayback.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMView.h"
#include "TrackWindowToolBar.h"

CommandTogglePlayback::CommandTogglePlayback(TrackWindowToolBar* p_opToolBar) :
m_opToolBar(p_opToolBar)
{
}

void CommandTogglePlayback::ExecuteE()
{
	if(m_opToolBar -> GetView() -> LockWindow())
	{
		if(*(static_cast<bool*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_IS_PLAYING, NULL, NULL, NULL))))
			m_opToolBar -> SetStop();
		else
			m_opToolBar -> SetPlay();
		m_opToolBar -> GetView() -> UnlockWindow();
	}

	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_TOGGLE_PLAYBACK);
/*
	if(m_opToolBar -> GetView() -> LockWindow())
	{
		if(*(static_cast<bool*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_IS_PLAYING, NULL, NULL, NULL))))
			m_opToolBar -> SetStop();
		else
			m_opToolBar -> SetPlay();
		m_opToolBar -> GetView() -> UnlockWindow();
	}

	bool vIsLocked(false);
	if(m_opToolBar -> GetView() -> WindowIsLocked())
	{
		vIsLocked = true;
		m_opToolBar -> GetView() -> UnlockWindow();
	}
	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_TOGGLE_PLAYBACK);
	if(vIsLocked)
		m_opToolBar -> GetView() -> LockWindow();
*/
}

bool CommandTogglePlayback::IsUndoable()
{
	return false;
}

bool CommandTogglePlayback::RequiresParameters()
{
	return false;
}


