#include "MediaCommandPrepareRecording.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaTimer.h"

MediaCommandPrepareRecording::MediaCommandPrepareRecording()
{
}

void MediaCommandPrepareRecording::ExecuteE()
{
	//;//cout_commented_out_4_release << "MediaCommandPrepareRecording" << endl;
	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();
	opProject -> SetArmed(! opProject -> IsArmed());
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
//	EMMediaTimer* opTimer = EMMediaTimer::Instance();

//	opTimer -> Start();
//	opTimer -> SeekToFrame(opTimer -> NowFrame());
//	opProject -> PrepareToPlayE();
//	opProject -> StartRecordingE();
}

bool MediaCommandPrepareRecording::RequiresParameters()
{
	return false;
}

bool MediaCommandPrepareRecording::IsUndoable()
{
	return false;
}