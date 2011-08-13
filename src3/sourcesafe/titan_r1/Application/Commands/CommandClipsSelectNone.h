/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_CLIPS_SELECT_NONE
#define __COMMAND_CLIPS_SELECT_NONE

#include "EMCommand.h"

class TrackWindow;

class CommandClipsSelectNone : public EMCommand
{
public:
	CommandClipsSelectNone(TrackWindow* p_opTrackWindow);
	void ExecuteE();
	bool IsUndoable();
	bool RequiresParameters();

private:
	TrackWindow* m_opTrackWindow;
};

#endif
