#include "MediaCommandPlayBuffer.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaTimer.h"

MediaCommandPlayBuffer::MediaCommandPlayBuffer()
{
}

void MediaCommandPlayBuffer::ExecuteE()
{
	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();
	opProject -> PlaySingleBuffer();
}

bool MediaCommandPlayBuffer::RequiresParameters()
{
	return false;
}

bool MediaCommandPlayBuffer::IsUndoable()
{
	return false;
}