#include "CommandExpandUtilityView.h"

#include "TrackWindow.h"

CommandExpandUtilityView::CommandExpandUtilityView(TrackWindow* p_opTrackWindow)
{
	m_opTrackWindow = p_opTrackWindow;
}

void CommandExpandUtilityView::ExecuteE()
{
	if(m_opTrackWindow -> GetWindow() -> Lock())
	{
		m_opTrackWindow -> ExpandUtilityView();
		m_opTrackWindow -> GetWindow() -> Unlock();
	}
}

bool CommandExpandUtilityView::IsUndoable()
{
	return false;
}

bool CommandExpandUtilityView::RequiresParameters()
{
	return false;
}

