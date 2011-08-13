#include "MediaCommandStartPlayback.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaTimer.h"

MediaCommandStartPlayback::MediaCommandStartPlayback()
{
}

void MediaCommandStartPlayback::ExecuteE()
{
	EMMediaProject* opProject = EMMediaEngine::Instance() -> GetMediaProject();
	EMMediaTimer* opTimer = EMMediaTimer::Instance();

	if(opTimer -> GetState() == EM_STOPPED)
		opTimer -> SetState((opProject -> IsArmed() ? EM_COUNTING_IN_FOR_RECORDING : EM_COUNTING_IN_FOR_PLAYBACK));
}

bool MediaCommandStartPlayback::RequiresParameters()
{
	return false;
}

bool MediaCommandStartPlayback::IsUndoable()
{
	return false;
}