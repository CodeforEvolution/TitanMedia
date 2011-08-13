#include "CommandToggleRelativeMove.h"

#include "SettingIDs.h"
#include "EMSettingsRepository.h"

CommandToggleRelativeMove::CommandToggleRelativeMove()
{
}

void CommandToggleRelativeMove::ExecuteE()
{
	bool vRelativeMove = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_RELATIVE)));
	if(vRelativeMove)
	{
		vRelativeMove = false;
		EMSettingsRepository::Instance() -> SetSetting(SETTING_RELATIVE, EM_SETTING_BOOL, &vRelativeMove);
	}
	else
	{
		vRelativeMove = true;
		EMSettingsRepository::Instance() -> SetSetting(SETTING_RELATIVE, EM_SETTING_BOOL, &vRelativeMove);
	}
}

bool CommandToggleRelativeMove::IsUndoable()
{
	return false;
}

bool CommandToggleRelativeMove::RequiresParameters()
{
	return false;
}


