#include "CommandRegionEnd.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMSettingsRepository.h"
#include "SettingIDs.h"
#include "TrackWindowToolBar.h"

CommandRegionEnd::CommandRegionEnd(TrackWindowToolBar* p_opToolBar) :
m_opToolBar(p_opToolBar)
{
}

void CommandRegionEnd::ExecuteE()
{
	m_opToolBar -> SetRegionEnd();
	int64* vpRegionEnd = static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_REGION_END));
	if(*vpRegionEnd != -1)
	{
		bool vIsPlaying(false);
		if(*(static_cast<bool*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_IS_PLAYING, NULL, NULL, NULL))))
		{
			vIsPlaying = true;
			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_STOP_PLAYBACK);
		}
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SEEK, vpRegionEnd);
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_PLAY_BUFFER);
		if(vIsPlaying)
			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_START_PLAYBACK);
	}
}

bool CommandRegionEnd::IsUndoable()
{
	return false;
}

bool CommandRegionEnd::RequiresParameters()
{
	return false;
}


