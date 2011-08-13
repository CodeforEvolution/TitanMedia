/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_TRACKS_SELECT_ALL
#define __COMMAND_TRACKS_SELECT_ALL

#include "EMCommand.h"

class TrackWindow;

class CommandTracksSelectAll : public EMCommand
{
public:
	CommandTracksSelectAll(TrackWindow* p_opTrackWindow);
	void ExecuteE();
	bool IsUndoable();
	bool RequiresParameters();

private:
	TrackWindow* m_opTrackWindow;
};

#endif
