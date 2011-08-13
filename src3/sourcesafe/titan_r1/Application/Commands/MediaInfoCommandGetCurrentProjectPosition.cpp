#include "MediaInfoCommandGetCurrentProjectPosition.h"

#include "EMMediaTimer.h"

MediaInfoCommandGetCurrentProjectPosition::MediaInfoCommandGetCurrentProjectPosition()
	:	m_vReturnValue(0)
{
}

void* MediaInfoCommandGetCurrentProjectPosition::ExecuteE(void*, void*, void*)
{
	m_vReturnValue = EMMediaTimer::Instance() -> NowTime();
	return static_cast<void*>(&m_vReturnValue);
}

bool MediaInfoCommandGetCurrentProjectPosition::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetCurrentProjectPosition::IsUndoable()
{
	return false;
}