#include "MediaInfoCommandIsPlaying.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMInputRepository.h"

MediaInfoCommandIsPlaying::MediaInfoCommandIsPlaying()
{
}

void* MediaInfoCommandIsPlaying::ExecuteE(void*, void*, void*)
{
	//eo << "MediaInfoCommandIsPlaying" << ef;
	EMMediaProject* opProject = EMMediaEngine::Instance() -> GetMediaProject();
	m_vReturnValue = opProject -> IsPlaying();
	return static_cast<void*>(&m_vReturnValue);
}

bool MediaInfoCommandIsPlaying::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandIsPlaying::IsUndoable()
{
	return false;
}
