#include "CommandZoomOutTrackVertical.h"

#include "EMGClipRepository.h"
#include "EMGTrack.h"
#include "EMGTrackRepository.h"
#include "EMGUIGlobals.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "TrackInfo.h"
#include "TrackView.h"
#include "TrackWindow.h"

CommandZoomOutTrackVertical::CommandZoomOutTrackVertical(TrackWindow* p_opTrackWindow)
{
	m_opTrackWindow = p_opTrackWindow;
}

void CommandZoomOutTrackVertical::ExecuteE()
{
	int32 vX;
	int32 vY;
	bool vButtonOne;
	bool vButtonTwo;
	bool vButtonThree;

	TrackInfo* opTrackInfo = m_opTrackWindow -> GetTrackInfo();
	TrackView* opTrackView = m_opTrackWindow -> GetTrackView();

	if(opTrackView -> GetView() -> LockWindow())
	{
		opTrackView -> GetView() -> GetMouse(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
	
		EMGTrack* opTrack = EMGTrackRepository::Instance() -> GetTrack(vX, vY);
		if(opTrack == NULL)
		{
			opTrackView -> GetView() -> UnlockWindow();
			return;
		}
		if(opTrack -> DecreaseZoomScale())
		{
			opTrackInfo -> CalculateTrackInfoFramesAndUpdate(NULL);
			opTrackView -> CalculateAndUpdateAll();
			if(static_cast<EMEditMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EDIT_MODE)))) == EM_TRACK_EDIT_MODE)
				EMGClipRepository::Instance() -> CalculateSelectedFrame();
		}

		// Only need to invalidate the affected track + all tracks below that track
		EMRect oInvalidateRect = opTrack -> GetClipContainer() -> Frame();
		oInvalidateRect.m_vBottom = (opTrackView -> GetView() -> Bounds()).m_vBottom;
		oInvalidateRect.m_vRight = (opTrackView -> GetView() -> Bounds()).m_vRight;
		opTrackView -> GetView() -> Invalidate(oInvalidateRect);

		opTrackView -> GetView() -> UnlockWindow();
	}
}

bool CommandZoomOutTrackVertical::IsUndoable()
{
	return false;
}

bool CommandZoomOutTrackVertical::RequiresParameters()
{
	return false;
}


