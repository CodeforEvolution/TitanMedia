#include "CommandExpandTimeLine.h"

#include "TrackWindow.h"

#include "EMGClipRepository.h" // Debug

CommandExpandTimeLine::CommandExpandTimeLine(TrackWindow* p_opTrackWindow)
{
	m_opTrackWindow = p_opTrackWindow;
}

void CommandExpandTimeLine::ExecuteE()
{
//	if(m_opTrackWindow -> GetWindow() -> Lock())
	{
		m_opTrackWindow -> ExpandTimeLine();
//		m_opTrackWindow -> GetWindow() -> Unlock();
	}
//EMGClipRepository::Instance() -> PrintStatus(); // Debug
}

bool CommandExpandTimeLine::IsUndoable()
{
	return false;
}

bool CommandExpandTimeLine::RequiresParameters()
{
	return false;
}

