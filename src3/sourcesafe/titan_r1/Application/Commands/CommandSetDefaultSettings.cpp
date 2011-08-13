#include "CommandSetDefaultSettings.h"

#include "EMGUIGlobals.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMVideoPreview.h"
#include "GUIGlobals.h"
#include "SettingIDs.h"

CommandSetDefaultSettings::CommandSetDefaultSettings()
{
}

// Do not put system settings in here (use CommandSetDefaultSystemSettings)
void CommandSetDefaultSettings::ExecuteE()
{
	float vFloatSetting;
	string oStringSetting;
	int64 vInt64Setting;
	int32 vInt32Setting;
	bool vBoolSetting;
	int vIntSetting;

	// Important that none of these SetSetting calls sends a Notify
	vFloatSetting = 25.0;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_VIDEO_FRAMERATE, EM_SETTING_FLOAT, &vFloatSetting, false);
	oStringSetting = "4/4";
	EMSettingsRepository::Instance() -> SetSetting(SETTING_SIGNATURE, EM_SETTING_STRING, &oStringSetting, false);
	vInt64Setting = 100000;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_TIME_ZOOM_SCALE, EM_SETTING_INT64, &vInt64Setting, false);
	vFloatSetting = 120;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_TEMPO, EM_SETTING_FLOAT, &vFloatSetting, false);
	vInt32Setting = 44100;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_AUDIO_FRAMERATE, EM_SETTING_INT32, &vInt32Setting, false);
	vBoolSetting = false;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_RELATIVE, EM_SETTING_BOOL, &vBoolSetting, false);
	oStringSetting = "Snap Clips";
	EMSettingsRepository::Instance() -> SetSetting(SETTING_SNAPPING_MODE, EM_SETTING_STRING, &oStringSetting, false);
	vBoolSetting = true;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_SNAPPING, EM_SETTING_BOOL, &vBoolSetting, false);
	vIntSetting = EM_TRACK_EDIT_MODE;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_EDIT_MODE, EM_SETTING_INT, &vIntSetting, false);
	vIntSetting = TOOLBOX_SELECT;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_TOOLBOX_MODE, EM_SETTING_INT, &vIntSetting, false);
	vInt64Setting = -1;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_REGION_BEGIN, EM_SETTING_INT64, &vInt64Setting, false);
	vInt64Setting = -1;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_REGION_END, EM_SETTING_INT64, &vInt64Setting, false);
	vBoolSetting = false;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_LOOP, EM_SETTING_BOOL, &vBoolSetting, false);
	vBoolSetting = true;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_AUDIO_ENABLED, EM_SETTING_BOOL, &vBoolSetting, false);
	vBoolSetting = true;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_VIDEO_ENABLED, EM_SETTING_BOOL, &vBoolSetting, false);
	vBoolSetting = true;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_MIDI_ENABLED, EM_SETTING_BOOL, &vBoolSetting, false);
	vBoolSetting = false;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_EXPERT_MODE, EM_SETTING_BOOL, &vBoolSetting, false);
	vIntSetting = 304;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_VIDEO_FORMAT_WIDTH, EM_SETTING_INT, &vIntSetting, false);
	vIntSetting = 168;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_VIDEO_FORMAT_HEIGHT, EM_SETTING_INT, &vIntSetting, false);
	vIntSetting = 200;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_VIDEO_RENDER_FORMAT_HEIGHT, EM_SETTING_INT, &vIntSetting, false);
	vIntSetting = 320;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_VIDEO_RENDER_FORMAT_WIDTH, EM_SETTING_INT, &vIntSetting, false);
	vFloatSetting = 25;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_VIDEO_RENDER_FRAMERATE, EM_SETTING_FLOAT, &vFloatSetting, false);
	oStringSetting = "unknown";
	EMSettingsRepository::Instance() -> SetSetting(SETTING_VIDEO_RENDER_CODEC, EM_SETTING_STRING, &oStringSetting, false);
	oStringSetting = "unknown";
	EMSettingsRepository::Instance() -> SetSetting(SETTING_AUDIO_RENDER_CODEC, EM_SETTING_STRING, &oStringSetting, false);
	oStringSetting = "unknown";
	EMSettingsRepository::Instance() -> SetSetting(SETTING_RENDER_CODEC_FAMILY, EM_SETTING_STRING, &oStringSetting, false);
	vIntSetting = 100;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_RENDER_CODEC_QUALITY, EM_SETTING_INT, &vIntSetting, false);
	vBoolSetting = false;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_MIDI_METRONOME_ENABLED, EM_SETTING_BOOL, &vBoolSetting, false);
	vBoolSetting = true;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_VIEW_CLIP_CONTENT_ENABLED, EM_SETTING_BOOL, &vBoolSetting, false);
	vBoolSetting = true;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_SIGNAL_METERS_ENABLED, EM_SETTING_BOOL, &vBoolSetting, false);
	vBoolSetting = true;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_AUDIO_LOCKED_TO_TEMPO, EM_SETTING_BOOL, &vBoolSetting, false);
	vBoolSetting = false;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_VIDEO_LOCKED_TO_TEMPO, EM_SETTING_BOOL, &vBoolSetting, false);
	vIntSetting = 100;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_VIDEO_RENDER_QUALITY, EM_SETTING_INT, &vIntSetting, false);
	vBoolSetting = false;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_MIDI_AUTO_QUANTIZE_ENABLED, EM_SETTING_BOOL, &vBoolSetting, false);
	oStringSetting = "1";
	EMSettingsRepository::Instance() -> SetSetting(SETTING_MIDI_AUTO_QUANTIZE_MODE, EM_SETTING_STRING, &oStringSetting, false);
	vBoolSetting = false;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_MIDI_AUTO_QUANTIZE_LENGTH_ENABLED, EM_SETTING_BOOL, &vBoolSetting, false);
	vInt32Setting = 0x00FFFFFF; //White
	EMSettingsRepository::Instance() -> SetSetting(SETTING_VIDEO_BACKGROUND_COLOR, EM_SETTING_INT32, &vInt32Setting, false);
	vBoolSetting = false;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_USE_PREDEFINED_VIDEO_FORMAT, EM_SETTING_BOOL, &vBoolSetting, false);
	vBoolSetting = false;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_USE_ASPECT_RATIOS, EM_SETTING_BOOL, &vBoolSetting, false);
	vBoolSetting = true;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_USE_PREDEFINED_VIDEO_FRAMERATE, EM_SETTING_BOOL, &vBoolSetting, false);
	vIntSetting = 0;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_ASPECT_RATIOS, EM_SETTING_INT, &vIntSetting, false);
	vBoolSetting = false;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_MIDI_METRONOME_USE_PC_SPEAKER, EM_SETTING_BOOL, &vBoolSetting, false);
	vBoolSetting = false;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_MIDI_METRONOME_USE_MIDI_NOTE, EM_SETTING_BOOL, &vBoolSetting, false);
	oStringSetting = "";
	EMSettingsRepository::Instance() -> SetSetting(SETTING_MIDI_METRONOME_MIDI_DEVICE, EM_SETTING_STRING, &oStringSetting, false);
	vInt32Setting = 15;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_MIDI_METRONOME_MIDI_CHANNEL, EM_SETTING_INT32, &vInt32Setting, false);
	vIntSetting = 45;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_MIDI_METRONOME_MIDI_NOTE_HIGH_KEY, EM_SETTING_INT, &vIntSetting, false);
	vIntSetting = 110;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_MIDI_METRONOME_MIDI_NOTE_HIGH_VELOCITY, EM_SETTING_INT, &vIntSetting, false);
	vInt64Setting = 50000;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_MIDI_METRONOME_MIDI_NOTE_HIGH_DURATION, EM_SETTING_INT64, &vInt64Setting, false);
	vIntSetting = 45;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_MIDI_METRONOME_MIDI_NOTE_LOW_KEY, EM_SETTING_INT, &vIntSetting, false);
	vIntSetting = 30;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_MIDI_METRONOME_MIDI_NOTE_LOW_VELOCITY, EM_SETTING_INT, &vIntSetting, false);
	vInt64Setting = 50000;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_MIDI_METRONOME_MIDI_NOTE_LOW_DURATION, EM_SETTING_INT64, &vInt64Setting, false);
	vInt32Setting = 0;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_MIDI_METRONOME_COUNT_IN, EM_SETTING_INT32, &vInt32Setting, false);
	vBoolSetting = false;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_MIDI_METRONOME_DURING_PLAYBACK, EM_SETTING_BOOL, &vBoolSetting, false);
	vBoolSetting = true;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_MIDI_METRONOME_DURING_RECORDING, EM_SETTING_BOOL, &vBoolSetting, false);

}

bool CommandSetDefaultSettings::IsUndoable()
{
	return false;
}

bool CommandSetDefaultSettings::RequiresParameters()
{
	return false;
}


