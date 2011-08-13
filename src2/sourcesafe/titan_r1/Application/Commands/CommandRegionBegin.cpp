#include "CommandRegionBegin.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMSettingsRepository.h"
#include "SettingIDs.h"
#include "TrackWindowToolBar.h"

CommandRegionBegin::CommandRegionBegin(TrackWindowToolBar* p_opToolBar) :
m_opToolBar(p_opToolBar)
{
}

void CommandRegionBegin::ExecuteE()
{
	m_opToolBar -> SetRegionBegin();
	int64* vpRegionBegin = static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_REGION_BEGIN));
	if(*vpRegionBegin != -1)
	{
		bool vIsPlaying(false);
		if(*(static_cast<bool*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_IS_PLAYING, NULL, NULL, NULL))))
		{
			vIsPlaying = true;
			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_STOP_PLAYBACK);
		}
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SEEK, vpRegionBegin);
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_PLAY_BUFFER);
		if(vIsPlaying)
			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_START_PLAYBACK);
	}
}

bool CommandRegionBegin::IsUndoable()
{
	return false;
}

bool CommandRegionBegin::RequiresParameters()
{
	return false;
}


