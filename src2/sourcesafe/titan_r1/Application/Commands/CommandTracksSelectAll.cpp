#include "CommandTracksSelectAll.h"

#include "EMGTrack.h"
#include "EMGTrackRepository.h"
#include "TrackInfo.h"
#include "TrackWindow.h"

#include <vector>

CommandTracksSelectAll::CommandTracksSelectAll(TrackWindow* p_opTrackWindow) :
m_opTrackWindow(p_opTrackWindow)
{
}

void CommandTracksSelectAll::ExecuteE()
{
	if(m_opTrackWindow -> GetTrackInfo() -> GetView() -> LockWindow())
	{
		vector<EMGTrack*>* opTracks = EMGTrackRepository::Instance() -> GetTracks();
		for(int vIndex = 0; vIndex < opTracks -> size(); vIndex++)
			(*opTracks)[vIndex] -> GetTrackInfo() -> SetSelected(true);	

		m_opTrackWindow -> GetTrackInfo() -> GetView() -> Invalidate(m_opTrackWindow -> GetTrackInfo() -> GetView() -> Bounds());

		m_opTrackWindow -> GetTrackInfo() -> EnableMenuItems();

		m_opTrackWindow -> GetTrackInfo() -> GetView() -> UnlockWindow();
	}
}

bool CommandTracksSelectAll::IsUndoable()
{
	return false;
}

bool CommandTracksSelectAll::RequiresParameters()
{
	return false;
}

