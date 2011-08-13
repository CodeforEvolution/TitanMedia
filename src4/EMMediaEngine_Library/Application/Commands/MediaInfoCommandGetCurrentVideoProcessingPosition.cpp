#include "MediaInfoCommandGetCurrentVideoProcessingPosition.h"

#include "EMMediaTimer.h"

MediaInfoCommandGetCurrentVideoProcessingPosition::MediaInfoCommandGetCurrentVideoProcessingPosition()
	:	m_vReturnValue(0)
{
}

void* MediaInfoCommandGetCurrentVideoProcessingPosition::ExecuteE(void*, void*, void*)
{
	m_vReturnValue = EMMediaTimer::Instance() -> VideoThenTime();
	return static_cast<void*>(&m_vReturnValue);
}

bool MediaInfoCommandGetCurrentVideoProcessingPosition::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetCurrentVideoProcessingPosition::IsUndoable()
{
	return false;
}