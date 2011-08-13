#include "CommandAudioLockToTempo.h"

#include "EMMenu.h"
#include "EMMenuBar.h"
#include "EMMenuItem.h"
#include "EMResourceRepository.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "ResourceIDs.h"

#include <string>

CommandAudioLockToTempo::CommandAudioLockToTempo()
{
}

void CommandAudioLockToTempo::ExecuteE()
{
	bool vLockAudioToTempo = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_AUDIO_LOCKED_TO_TEMPO)));
	vLockAudioToTempo = !vLockAudioToTempo;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_AUDIO_LOCKED_TO_TEMPO, EM_SETTING_BOOL, &vLockAudioToTempo);
}

bool CommandAudioLockToTempo::IsUndoable()
{
	return false;
}

bool CommandAudioLockToTempo::RequiresParameters()
{
	return false;
}


