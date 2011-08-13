#include "CommandStop.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMThread.h"
#include "EMView.h"
#include "TrackWindowToolBar.h"

CommandStop::CommandStop(TrackWindowToolBar* p_opToolBar) :
m_opToolBar(p_opToolBar)
{
}

// If the media engine needs to call this method, it should do it through COMMAND_POST_COMMAND
void CommandStop::ExecuteE()
{
	if(m_opToolBar -> GetView() -> LockWindow())
	{
		m_opToolBar -> SetStop();
		m_opToolBar -> GetView() -> UnlockWindow();
	}

	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_STOP_PLAYBACK);
/*
	// FIXME: These temporary unlocks/locks are very hackish (in other commands as well). Must be fixed!
	// This lock is needed since the media system calls this command when rendering is completed
	if(m_opToolBar -> GetView() -> LockWindow())
	{
		m_opToolBar -> SetStop();
		m_opToolBar -> GetView() -> UnlockWindow();
	}

	bool vIsLocked(false);
	if(m_opToolBar -> GetView() -> WindowIsLocked())
	{
		vIsLocked = true;
		m_opToolBar -> GetView() -> UnlockWindow();
	}
	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_STOP_PLAYBACK);
	if(vIsLocked)
		m_opToolBar -> GetView() -> LockWindow();
*/
}

bool CommandStop::IsUndoable()
{
	return false;
}

bool CommandStop::RequiresParameters()
{
	return false;
}


