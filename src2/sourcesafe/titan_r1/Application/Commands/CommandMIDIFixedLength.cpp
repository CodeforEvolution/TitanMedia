#include "CommandMIDIFixedLength.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGClip.h"
#include "EMGClipRepository.h"
#include "TrackView.h"
#include "TrackWindow.h"

#include <list>

CommandMIDIFixedLength::CommandMIDIFixedLength(TrackWindow* p_opTrackWindow) :
m_opTrackWindow(p_opTrackWindow)
{
}

void CommandMIDIFixedLength::ExecuteE()
{
	if(m_opTrackWindow -> GetTrackInfo() -> GetView() -> LockWindow())
	{
		// FIXME: Do stuff here
		m_opTrackWindow -> GetTrackInfo() -> GetView() -> UnlockWindow();
	}
}

bool CommandMIDIFixedLength::IsUndoable()
{
	return false;
}

bool CommandMIDIFixedLength::RequiresParameters()
{
	return false;
}

