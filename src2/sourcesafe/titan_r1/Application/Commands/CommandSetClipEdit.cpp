#include "CommandSetClipEdit.h"

#include "EMGUIGlobals.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "GUIGlobals.h"
#include "SettingIDs.h"

CommandSetClipEdit::CommandSetClipEdit()
{
}

void CommandSetClipEdit::ExecuteE()
{
	int vIntSetting = TOOLBOX_SELECT;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_TOOLBOX_MODE, EM_SETTING_INT, &vIntSetting, false);
	vIntSetting = EM_CLIP_EDIT_MODE;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_EDIT_MODE, EM_SETTING_INT, &vIntSetting);
}

bool CommandSetClipEdit::IsUndoable()
{
	return false;
}

bool CommandSetClipEdit::RequiresParameters()
{
	return false;
}

