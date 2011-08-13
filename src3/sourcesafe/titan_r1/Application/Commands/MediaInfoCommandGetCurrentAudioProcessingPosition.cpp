#include "MediaInfoCommandGetCurrentAudioProcessingPosition.h"

#include "EMMediaTimer.h"

MediaInfoCommandGetCurrentAudioProcessingPosition::MediaInfoCommandGetCurrentAudioProcessingPosition()
	:	m_vReturnValue(0)
{
}

void* MediaInfoCommandGetCurrentAudioProcessingPosition::ExecuteE(void*, void*, void*)
{
	m_vReturnValue = EMMediaTimer::Instance() -> AudioThenTime();
	return static_cast<void*>(&m_vReturnValue);
}

bool MediaInfoCommandGetCurrentAudioProcessingPosition::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetCurrentAudioProcessingPosition::IsUndoable()
{
	return false;
}