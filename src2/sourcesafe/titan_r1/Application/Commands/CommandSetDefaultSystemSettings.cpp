#include "CommandSetDefaultSystemSettings.h"

#include "EMGUIGlobals.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMVideoPreview.h"
#include "GUIGlobals.h"
#include "SettingIDs.h"

CommandSetDefaultSystemSettings::CommandSetDefaultSystemSettings()
{
}

void CommandSetDefaultSystemSettings::ExecuteE()
{
	//float vFloatSetting;
	string oStringSetting;
	int64 vInt64Setting;
	int32 vInt32Setting;
	bool vBoolSetting;
	//int vIntSetting;

	// Important that none of these SetSetting calls sends a Notify
	vInt32Setting = 10;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_SNAP_THRESHOLD, EM_SETTING_INT32, &vInt32Setting, false, true);
	vInt32Setting = 200;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_AUDIO_PROCESSING_AHEAD, EM_SETTING_INT32, &vInt32Setting, false, true);
	vInt32Setting = 3676;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_AUDIO_BUFFER_SIZE, EM_SETTING_INT32, &vInt32Setting, false, true);
	oStringSetting = "server.sonork.com";
	EMSettingsRepository::Instance() -> SetSetting(SETTING_NETWORK_HOST, EM_SETTING_STRING, &oStringSetting, false, true);
	vBoolSetting = true;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_NETWORK_USE_TCP, EM_SETTING_BOOL, &vBoolSetting, false, true);
	vInt32Setting = 1504;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_NETWORK_TCP_PORT, EM_SETTING_INT32, &vInt32Setting, false, true);
	vInt32Setting = 1503;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_NETWORK_UDP_PORT, EM_SETTING_INT32, &vInt32Setting, false, true);
	vBoolSetting = true;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_NETWORK_USE_INTERNET, EM_SETTING_BOOL, &vBoolSetting, false, true);
	vInt64Setting = 0;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_NETWORK_USER_ID, EM_SETTING_INT64, &vInt64Setting, false, true);
	oStringSetting = "";
	EMSettingsRepository::Instance() -> SetSetting(SETTING_NETWORK_USER_PASSWORD, EM_SETTING_STRING, &oStringSetting, false, true);
	vBoolSetting = false;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_NETWORK_SAVE_PASSWORD, EM_SETTING_BOOL, &vBoolSetting, false, true);
	vBoolSetting = true;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_SOFTWARE_IS_CRIPPLED, EM_SETTING_BOOL, &vBoolSetting, false, true);
	oStringSetting = "Unregistered User";
	EMSettingsRepository::Instance() -> SetSetting(SETTING_REGISTRATION_USER_ID, EM_SETTING_STRING, &oStringSetting, false, true);
	vInt64Setting = -1;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_REGISTRATION_SERIAL_NUMBER, EM_SETTING_INT64, &vInt64Setting, false, true);
}

bool CommandSetDefaultSystemSettings::IsUndoable()
{
	return false;
}

bool CommandSetDefaultSystemSettings::RequiresParameters()
{
	return false;
}


