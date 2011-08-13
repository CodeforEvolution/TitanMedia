#include "CommandTracksSelectInvert.h"

#include "EMGTrack.h"
#include "EMGTrackRepository.h"
#include "TrackInfo.h"
#include "TrackWindow.h"

#include <vector>

CommandTracksSelectInvert::CommandTracksSelectInvert(TrackWindow* p_opTrackWindow) :
m_opTrackWindow(p_opTrackWindow)
{
}

void CommandTracksSelectInvert::ExecuteE()
{
	if(m_opTrackWindow -> GetTrackInfo() -> GetView() -> LockWindow())
	{
		vector<EMGTrack*>* opAllTracks = EMGTrackRepository::Instance() -> GetTracks();
		vector<EMGTrack*> oSelectedTracks = EMGTrackRepository::Instance() -> GetSelectedTracks();
		for(int vIndex = 0; vIndex < opAllTracks -> size(); vIndex++)
			(*opAllTracks)[vIndex] -> GetTrackInfo() -> SetSelected(true);
		for(vIndex = 0; vIndex < oSelectedTracks.size(); vIndex++)
			oSelectedTracks[vIndex] -> GetTrackInfo() -> SetSelected(false);

		m_opTrackWindow -> GetTrackInfo() -> GetView() -> Invalidate(m_opTrackWindow -> GetTrackInfo() -> GetView() -> Bounds());

		m_opTrackWindow -> GetTrackInfo() -> EnableMenuItems();

		m_opTrackWindow -> GetTrackInfo() -> GetView() -> UnlockWindow();
	}
}

bool CommandTracksSelectInvert::IsUndoable()
{
	return false;
}

bool CommandTracksSelectInvert::RequiresParameters()
{
	return false;
}

