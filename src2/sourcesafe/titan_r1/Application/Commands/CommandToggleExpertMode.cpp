#include "CommandToggleExpertMode.h"

#include "EMMenu.h"
#include "EMMenuBar.h"
#include "EMMenuItem.h"
#include "EMResourceRepository.h"
#include "EMSettingsRepository.h"
#include "EMView.h"
#include "ResourceIDs.h"
#include "SettingIDs.h"
#include "TrackView.h"

#include <string>

CommandToggleExpertMode::CommandToggleExpertMode(EMMenuBar* p_opMenuBar, TrackView* p_opTrackView) :
m_opMenuBar(p_opMenuBar),
m_opTrackView(p_opTrackView)
{
}

void CommandToggleExpertMode::ExecuteE()
{
	bool vExpertMode = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EXPERT_MODE)));
	vExpertMode = !vExpertMode;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_EXPERT_MODE, EM_SETTING_BOOL, &vExpertMode);

	if(m_opTrackView -> GetView() -> LockWindow())
	{
		m_opTrackView -> GetView() -> Invalidate(m_opTrackView -> GetView() -> Bounds());
		m_opTrackView -> GetView() -> UnlockWindow();
	}
/*
	string* opMenuName = EMResourceRepository::GetString(RES_TM_USING);
	string* opItemName = EMResourceRepository::GetString(RES_TM_USING_ADVANCED_EDITING);
	EMMenu* opMenu = m_opMenuBar -> GetMenu(opMenuName);
	EMMenuItem* opMenuItem = opMenu -> GetMenuItem(opItemName);
	opMenuItem -> SetMarked(vExpertMode);
*/
}

bool CommandToggleExpertMode::IsUndoable()
{
	return false;
}

bool CommandToggleExpertMode::RequiresParameters()
{
	return false;
}


