/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_ZOOM_IN_TRACK_VERTICAL
#define __COMMAND_ZOOM_IN_TRACK_VERTICAL

#include "EMCommand.h"

class TrackWindow;

class CommandZoomInTrackVertical : public EMCommand
{
public:
	CommandZoomInTrackVertical(TrackWindow* p_opTrackWindow);
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();

private:
	TrackWindow* m_opTrackWindow;
};

#endif
