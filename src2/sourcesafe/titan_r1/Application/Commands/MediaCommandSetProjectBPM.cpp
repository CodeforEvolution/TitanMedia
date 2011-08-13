#include "MediaCommandSetProjectBPM.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaMIDIEventRepository.h"
#include "EMWinAudioClipRepository.h"

MediaCommandSetProjectBPM::MediaCommandSetProjectBPM()
{
}

void* MediaCommandSetProjectBPM::ExecuteE(void* p_upBPM, void*, void*)
{
	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();
	uint32 vBPM = (uint32) *(static_cast<float*>(p_upBPM));

	opProject -> GetMIDIEventRepository() -> AdjustToBPM(vBPM);
	static_cast<EMWinAudioClipRepository*>(opProject -> GetAudioClipRepository()) -> AdjustToBPM(vBPM);

	return NULL;
}

bool MediaCommandSetProjectBPM::RequiresParameters()
{
	return true;
}

bool MediaCommandSetProjectBPM::IsUndoable()
{
	return false;
}
