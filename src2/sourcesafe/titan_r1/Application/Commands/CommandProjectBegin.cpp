#include "CommandProjectBegin.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "TrackWindowToolBar.h"

CommandProjectBegin::CommandProjectBegin(TrackWindowToolBar* p_opToolBar) :
m_opToolBar(p_opToolBar)
{
}

void CommandProjectBegin::ExecuteE()
{
	m_opToolBar -> SetProjectBegin();
	int64* vpPosition = new int64(0);
	bool vIsPlaying(false);
	if(*(static_cast<bool*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_IS_PLAYING, NULL, NULL, NULL))))
	{
		vIsPlaying = true;
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_STOP_PLAYBACK);
	}
	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SEEK, vpPosition);
	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_PLAY_BUFFER);
	if(vIsPlaying)
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_START_PLAYBACK);
	delete vpPosition;
}

bool CommandProjectBegin::IsUndoable()
{
	return false;
}

bool CommandProjectBegin::RequiresParameters()
{
	return false;
}


