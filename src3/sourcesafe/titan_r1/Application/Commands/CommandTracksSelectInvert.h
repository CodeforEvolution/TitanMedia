/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_TRACKS_SELECT_INVERT
#define __COMMAND_TRACKS_SELECT_INVERT

#include "EMCommand.h"

class TrackWindow;

class CommandTracksSelectInvert : public EMCommand
{
public:
	CommandTracksSelectInvert(TrackWindow* p_opTrackWindow);
	void ExecuteE();
	bool IsUndoable();
	bool RequiresParameters();

private:
	TrackWindow* m_opTrackWindow;
};

#endif
