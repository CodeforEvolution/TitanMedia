#include "MediaCommandSetRenderingFlags.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"

MediaCommandSetRenderingFlags::MediaCommandSetRenderingFlags()
{
}

void* MediaCommandSetRenderingFlags::ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree)
{
	bool vpShouldRenderVideo = *static_cast<bool*>(p_opParameterOne);
	bool vpShouldRenderAudio = *static_cast<bool*>(p_opParameterTwo);
	EMMediaEngine::Instance() ->GetMediaProject()->SetRenderingVideo(vpShouldRenderVideo);
	EMMediaEngine::Instance() ->GetMediaProject()->SetRenderingAudio(vpShouldRenderAudio);
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
	return NULL;
}

bool MediaCommandSetRenderingFlags::RequiresParameters()
{
	return true;
}

bool MediaCommandSetRenderingFlags::IsUndoable()
{
	return false;
}