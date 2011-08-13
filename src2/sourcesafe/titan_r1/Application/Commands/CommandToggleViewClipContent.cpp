#include "CommandToggleViewClipContent.h"

#include "EMMenu.h"
#include "EMMenuBar.h"
#include "EMMenuItem.h"
#include "EMResourceRepository.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "ResourceIDs.h"

#include <string>

CommandToggleViewClipContent::CommandToggleViewClipContent(EMMenuBar* p_opMenuBar) :
m_opMenuBar(p_opMenuBar)
{
}

void CommandToggleViewClipContent::ExecuteE()
{
	string* opMenuName = EMResourceRepository::GetString(RES_TM_VIEW);
	string* opItemName = EMResourceRepository::GetString(RES_TM_CLIP_CONTENTS);
	EMMenu* opMenu = m_opMenuBar -> GetMenu(opMenuName);
	EMMenuItem* opMenuItem = opMenu -> GetMenuItem(opItemName);
	bool vClipContent = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIEW_CLIP_CONTENT_ENABLED)));
	vClipContent = !vClipContent;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_VIEW_CLIP_CONTENT_ENABLED, EM_SETTING_BOOL, &vClipContent);
	opMenuItem -> SetMarked(vClipContent);
}

bool CommandToggleViewClipContent::IsUndoable()
{
	return false;
}

bool CommandToggleViewClipContent::RequiresParameters()
{
	return false;
}


