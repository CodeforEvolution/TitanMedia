#include "CommandToggleEnableVideo.h"

#include "EMMenu.h"
#include "EMMenuBar.h"
#include "EMMenuItem.h"
#include "EMResourceRepository.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMView.h"
#include "ResourceIDs.h"
#include "TrackView.h"

#include <string>

CommandToggleEnableVideo::CommandToggleEnableVideo(EMMenuBar* p_opMenuBar, TrackView* p_opTrackView) :
m_opMenuBar(p_opMenuBar),
m_opTrackView(p_opTrackView)
{
}

void CommandToggleEnableVideo::ExecuteE()
{
	bool vEnableVideo = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_ENABLED)));
	vEnableVideo = !vEnableVideo;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_VIDEO_ENABLED, EM_SETTING_BOOL, &vEnableVideo);

	if(m_opTrackView -> GetView() -> LockWindow())
	{
		m_opTrackView -> GetView() -> Invalidate(m_opTrackView -> GetView() -> Bounds());
		m_opTrackView -> GetView() -> UnlockWindow();
	}
/*
	string* opMenuName = EMResourceRepository::GetString(RES_TM_USING);
	string* opItemName = EMResourceRepository::GetString(RES_TM_USING_VIDEO);
	EMMenu* opMenu = m_opMenuBar -> GetMenu(opMenuName);
	EMMenuItem* opMenuItem = opMenu -> GetMenuItem(opItemName);
	opMenuItem -> SetMarked(vEnableVideo);

	opMenuName = EMResourceRepository::GetString(RES_TM_VIDEO);
	opMenu = m_opMenuBar -> GetMenu(opMenuName);
	opMenu -> SetEnabled(vEnableVideo);

	opMenuName = EMResourceRepository::GetString(RES_TM_TRACK);
	opItemName = EMResourceRepository::GetString(RES_TM_CREATE_VIDEO_TRACK);
	opMenu = m_opMenuBar -> GetMenu(opMenuName);
	opMenuItem = opMenu -> GetMenuItem(opItemName);
	opMenuItem -> SetEnabled(vEnableVideo);
*/
}

bool CommandToggleEnableVideo::IsUndoable()
{
	return false;
}

bool CommandToggleEnableVideo::RequiresParameters()
{
	return false;
}


