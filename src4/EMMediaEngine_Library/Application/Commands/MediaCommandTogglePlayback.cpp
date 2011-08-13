#include "MediaCommandTogglePlayback.h"

#include "EMMediaEngine.h"
#include "EMMediaTimer.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"

MediaCommandTogglePlayback::MediaCommandTogglePlayback()
{
}

void MediaCommandTogglePlayback::ExecuteE()
{
	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaTimer* opTimer = EMMediaTimer::Instance();

	if(opTimer -> GetState() == EM_STOPPED)
		opEngine -> GetCommandRepository() -> ExecuteCommand(MEDIA_COMMAND_START_PLAYBACK);
	else
		opEngine -> GetCommandRepository() -> ExecuteCommand(MEDIA_COMMAND_STOP_PLAYBACK);
}

bool MediaCommandTogglePlayback::RequiresParameters()
{
	return false;
}

bool MediaCommandTogglePlayback::IsUndoable()
{
	return false;
}

