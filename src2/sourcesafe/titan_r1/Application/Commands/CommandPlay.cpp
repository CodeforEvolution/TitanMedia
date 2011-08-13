#include "CommandPlay.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "TrackWindowToolBar.h"

CommandPlay::CommandPlay(TrackWindowToolBar* p_opToolBar) :
m_opToolBar(p_opToolBar)
{
}

void CommandPlay::ExecuteE()
{
	m_opToolBar -> SetPlay();
	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_START_PLAYBACK);
}

bool CommandPlay::IsUndoable()
{
	return false;
}

bool CommandPlay::RequiresParameters()
{
	return false;
}


