/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_ZOOM_OUT_VERTICAL
#define __COMMAND_ZOOM_OUT_VERTICAL

#include "EMCommand.h"

class TrackWindow;

class CommandZoomOutTrackVertical : public EMCommand
{
public:
	CommandZoomOutTrackVertical(TrackWindow* p_opTrackWindow);
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();

private:
	TrackWindow* m_opTrackWindow;
};

#endif
