#include "CommandSetFXEdit.h"

#include "EMGUIGlobals.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "GUIGlobals.h"
#include "SettingIDs.h"

CommandSetFXEdit::CommandSetFXEdit()
{
}

void CommandSetFXEdit::ExecuteE()
{
	int vIntSetting = TOOLBOX_SELECT;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_TOOLBOX_MODE, EM_SETTING_INT, &vIntSetting, false);
	vIntSetting = EM_FX_EDIT_MODE;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_EDIT_MODE, EM_SETTING_INT, &vIntSetting);
}

bool CommandSetFXEdit::IsUndoable()
{
	return false;
}

bool CommandSetFXEdit::RequiresParameters()
{
	return false;
}

