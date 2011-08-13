#include "CommandToggleEnableAudio.h"

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

CommandToggleEnableAudio::CommandToggleEnableAudio(EMMenuBar* p_opMenuBar, TrackView* p_opTrackView) :
m_opMenuBar(p_opMenuBar),
m_opTrackView(p_opTrackView)
{
}

void CommandToggleEnableAudio::ExecuteE()
{
	bool vEnableAudio = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_AUDIO_ENABLED)));
	vEnableAudio = !vEnableAudio;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_AUDIO_ENABLED, EM_SETTING_BOOL, &vEnableAudio);

	if(m_opTrackView -> GetView() -> LockWindow())
	{
		m_opTrackView -> GetView() -> Invalidate(m_opTrackView -> GetView() -> Bounds());
		m_opTrackView -> GetView() -> UnlockWindow();
	}
/*
	string* opMenuName = EMResourceRepository::GetString(RES_TM_USING);
	string* opItemName = EMResourceRepository::GetString(RES_TM_USING_AUDIO);
	EMMenu* opMenu = m_opMenuBar -> GetMenu(opMenuName);
	EMMenuItem* opMenuItem = opMenu -> GetMenuItem(opItemName);
	opMenuItem -> SetMarked(vEnableAudio);

	opMenuName = EMResourceRepository::GetString(RES_TM_AUDIO);
	opMenu = m_opMenuBar -> GetMenu(opMenuName);
	opMenu -> SetEnabled(vEnableAudio);

	opMenuName = EMResourceRepository::GetString(RES_TM_TRACK);
	opItemName = EMResourceRepository::GetString(RES_TM_CREATE_AUDIO_TRACK);
	opMenu = m_opMenuBar -> GetMenu(opMenuName);
	opMenuItem = opMenu -> GetMenuItem(opItemName);
	opMenuItem -> SetEnabled(vEnableAudio);
*/
}

bool CommandToggleEnableAudio::IsUndoable()
{
	return false;
}

bool CommandToggleEnableAudio::RequiresParameters()
{
	return false;
}


