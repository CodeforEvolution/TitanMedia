#include "CommandTracksSelectNone.h"

#include "EMGTrack.h"
#include "EMGTrackRepository.h"
#include "TrackInfo.h"
#include "TrackWindow.h"

#include <vector>

CommandTracksSelectNone::CommandTracksSelectNone(TrackWindow* p_opTrackWindow) :
m_opTrackWindow(p_opTrackWindow)
{
}

void CommandTracksSelectNone::ExecuteE()
{
	if(m_opTrackWindow -> GetTrackInfo() -> GetView() -> LockWindow())
	{
		vector<EMGTrack*>* opTracks = EMGTrackRepository::Instance() -> GetTracks();
		for(int vIndex = 0; vIndex < opTracks -> size(); vIndex++)
			(*opTracks)[vIndex] -> GetTrackInfo() -> SetSelected(false);

		m_opTrackWindow -> GetTrackInfo() -> GetView() -> Invalidate(m_opTrackWindow -> GetTrackInfo() -> GetView() -> Bounds());

		m_opTrackWindow -> GetTrackInfo() -> EnableMenuItems();

		m_opTrackWindow -> GetTrackInfo() -> GetView() -> UnlockWindow();
	}
}

bool CommandTracksSelectNone::IsUndoable()
{
	return false;
}

bool CommandTracksSelectNone::RequiresParameters()
{
	return false;
}

