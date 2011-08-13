#include "CommandToggleSnapping.h"

#include "SettingIDs.h"
#include "EMSettingsRepository.h"

CommandToggleSnapping::CommandToggleSnapping()
{
}

void CommandToggleSnapping::ExecuteE()
{
	bool vSnapping = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SNAPPING)));
	if(vSnapping)
	{
		vSnapping = false;
		EMSettingsRepository::Instance() -> SetSetting(SETTING_SNAPPING, EM_SETTING_BOOL, &vSnapping);
	}
	else
	{
		vSnapping = true;
		EMSettingsRepository::Instance() -> SetSetting(SETTING_SNAPPING, EM_SETTING_BOOL, &vSnapping);
	}
}

bool CommandToggleSnapping::IsUndoable()
{
	return false;
}

bool CommandToggleSnapping::RequiresParameters()
{
	return false;
}
