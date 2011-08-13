#include "CommandZoomInTracksVertical.h"

#include "EMGClipRepository.h"
#include "EMGTrack.h"
#include "EMGTrackRepository.h"
#include "EMGUIGlobals.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "GUIGlobals.h"
#include "TrackInfo.h"
#include "TrackView.h"
#include "TrackWindow.h"

CommandZoomInTracksVertical::CommandZoomInTracksVertical(TrackWindow* p_opTrackWindow)
{
	m_opTrackWindow = p_opTrackWindow;
}

void CommandZoomInTracksVertical::ExecuteE()
{
	TrackInfo* opTrackInfo = m_opTrackWindow -> GetTrackInfo();
	TrackView* opTrackView = m_opTrackWindow -> GetTrackView();

	if(opTrackView -> GetView() -> LockWindow())
	{
		uint32 vIndex;
		for(vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
			EMGTrackRepository::Instance() -> GetTrack(vIndex) -> IncreaseZoomScale();
		opTrackInfo -> CalculateTrackInfoFramesAndUpdate(EMGTrackRepository::Instance() -> GetTrack(vIndex));
		opTrackView -> CalculateAndUpdateAll();
		
		if(static_cast<EMEditMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EDIT_MODE)))) == EM_TRACK_EDIT_MODE)
			EMGClipRepository::Instance() -> CalculateSelectedFrame();

		opTrackView -> GetView() -> Invalidate(opTrackView -> GetView() -> Bounds());

		opTrackView -> GetView() -> UnlockWindow();
	}
}

bool CommandZoomInTracksVertical::IsUndoable()
{
	return false;
}

bool CommandZoomInTracksVertical::RequiresParameters()
{
	return false;
}


