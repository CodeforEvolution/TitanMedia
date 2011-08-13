/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_CLIPS_SELECT_ALL
#define __COMMAND_CLIPS_SELECT_ALL

#include "EMCommand.h"

class TrackWindow;

class CommandClipsSelectAll : public EMCommand
{
public:
	CommandClipsSelectAll(TrackWindow* p_opTrackWindow);
	void ExecuteE();
	bool IsUndoable();
	bool RequiresParameters();

private:
	TrackWindow* m_opTrackWindow;
};

#endif
