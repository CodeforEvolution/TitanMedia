/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_ZOOM_OUT_TRACKS_VERTICAL
#define __COMMAND_ZOOM_OUT_TRACKS_VERTICAL

#include "EMCommand.h"

class TrackWindow;

class CommandZoomOutTracksVertical : public EMCommand
{
public:
	CommandZoomOutTracksVertical(TrackWindow* p_opTrackWindow);
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();

private:
	TrackWindow* m_opTrackWindow;
};

#endif
