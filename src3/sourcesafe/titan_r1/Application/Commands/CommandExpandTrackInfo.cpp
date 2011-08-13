#include "CommandExpandTrackInfo.h"

#include "TrackWindow.h"

CommandExpandTrackInfo::CommandExpandTrackInfo(TrackWindow* p_opTrackWindow)
{
	m_opTrackWindow = p_opTrackWindow;
}

void CommandExpandTrackInfo::ExecuteE()
{
	if(m_opTrackWindow -> GetWindow() -> Lock())
	{
		m_opTrackWindow -> ExpandTrackInfo();
		m_opTrackWindow -> GetWindow() -> Unlock();
	}
}

bool CommandExpandTrackInfo::IsUndoable()
{
	return false;
}

bool CommandExpandTrackInfo::RequiresParameters()
{
	return false;
}


