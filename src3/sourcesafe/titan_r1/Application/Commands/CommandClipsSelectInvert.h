/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_CLIPS_SELECT_INVERT
#define __COMMAND_CLIPS_SELECT_INVERT

#include "EMCommand.h"

class TrackWindow;

class CommandClipsSelectInvert : public EMCommand
{
public:
	CommandClipsSelectInvert(TrackWindow* p_opTrackWindow);
	void ExecuteE();
	bool IsUndoable();
	bool RequiresParameters();

private:
	TrackWindow* m_opTrackWindow;
};

#endif
