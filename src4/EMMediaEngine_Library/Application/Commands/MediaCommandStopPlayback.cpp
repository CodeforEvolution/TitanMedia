#include "MediaCommandStopPlayback.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaTimer.h"
#include "EMWinAudioDiskWriter.h"

MediaCommandStopPlayback::MediaCommandStopPlayback()
{
}

void MediaCommandStopPlayback::ExecuteE()
{
	//;//cout_commented_out_4_release << "MediaCommandStopPlayback" << endl;
	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();
	EMMediaTimer* opTimer = EMMediaTimer::Instance();

	//By stopping the timer, an event is sent to the outputs
	//that clears the pre-cached buffers
	//TODO: What happens with incoming, semaphore blocked, buffers?!

	if(opProject != NULL)
	{
		EMMediaTimerState eState = opTimer -> GetState();
		opTimer -> SetState(EM_STOPPED);
		EMWinAudioDiskWriter::Instance() -> Stop();
		if(eState == EM_RECORDING)
			opProject -> SetArmed(false); 
	}
}

bool MediaCommandStopPlayback::RequiresParameters()
{
	return false;
}

bool MediaCommandStopPlayback::IsUndoable()
{
	return false;
}
