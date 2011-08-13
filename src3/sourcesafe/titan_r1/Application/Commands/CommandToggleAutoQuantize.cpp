#include "CommandToggleAutoQuantize.h"

#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"

CommandToggleAutoQuantize::CommandToggleAutoQuantize()
{
}

void CommandToggleAutoQuantize::ExecuteE()
{
	bool vAutoQuantize = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_MIDI_AUTO_QUANTIZE_ENABLED)));
	if(vAutoQuantize)
	{
		vAutoQuantize = false;
		EMSettingsRepository::Instance() -> SetSetting(SETTING_MIDI_AUTO_QUANTIZE_ENABLED, EM_SETTING_BOOL, &vAutoQuantize);
	}
	else
	{
		vAutoQuantize = true;
		EMSettingsRepository::Instance() -> SetSetting(SETTING_MIDI_AUTO_QUANTIZE_ENABLED, EM_SETTING_BOOL, &vAutoQuantize);
	}
}

bool CommandToggleAutoQuantize::IsUndoable()
{
	return false;
}

bool CommandToggleAutoQuantize::RequiresParameters()
{
	return false;
}
