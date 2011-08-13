#include "CommandSetTrackEdit.h"

#include "EMGUIGlobals.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "SettingIDs.h"

CommandSetTrackEdit::CommandSetTrackEdit()
{
}

void CommandSetTrackEdit::ExecuteE()
{
	int vSetting = EM_TRACK_EDIT_MODE;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_EDIT_MODE, EM_SETTING_INT, &vSetting);
}

bool CommandSetTrackEdit::IsUndoable()
{
	return false;
}

bool CommandSetTrackEdit::RequiresParameters()
{
	return false;
}

