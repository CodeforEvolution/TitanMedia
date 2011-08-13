#include "CommandClipsSelectNone.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGClip.h"
#include "EMGClipRepository.h"
#include "TrackView.h"
#include "TrackWindow.h"

#include <list>

CommandClipsSelectNone::CommandClipsSelectNone(TrackWindow* p_opTrackWindow) :
m_opTrackWindow(p_opTrackWindow)
{
}

void CommandClipsSelectNone::ExecuteE()
{
	if(m_opTrackWindow -> GetTrackInfo() -> GetView() -> LockWindow())
	{
		EMCommandRepository::Instance() -> ExecuteCommand(UNDO_COMMAND_CHANGE_CLIP_SELECTION, EMGClipRepository::Instance() -> GetSelectedClips(), NULL, NULL);
		EMGClipRepository::Instance() -> SetSelected(EMGClipRepository::Instance() -> GetClips(), false);
		m_opTrackWindow -> GetTrackView() -> EnableMenuItems();
		m_opTrackWindow -> GetTrackView() -> GetView() -> Invalidate(m_opTrackWindow -> GetTrackView() -> GetView() -> Bounds());

		m_opTrackWindow -> GetTrackInfo() -> GetView() -> UnlockWindow();
	}
}

bool CommandClipsSelectNone::IsUndoable()
{
	return false;
}

bool CommandClipsSelectNone::RequiresParameters()
{
	return false;
}

