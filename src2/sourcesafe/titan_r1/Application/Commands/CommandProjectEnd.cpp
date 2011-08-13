#include "CommandProjectEnd.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGClipRepository.h"
#include "TrackWindowToolBar.h"

CommandProjectEnd::CommandProjectEnd(TrackWindowToolBar* p_opToolBar) :
m_opToolBar(p_opToolBar)
{
}

void CommandProjectEnd::ExecuteE()
{
	m_opToolBar -> SetProjectEnd();
	int64* vpPosition = new int64(EMGClipRepository::Instance() -> GetProjectEnd());
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

bool CommandProjectEnd::IsUndoable()
{
	return false;
}

bool CommandProjectEnd::RequiresParameters()
{
	return false;
}


