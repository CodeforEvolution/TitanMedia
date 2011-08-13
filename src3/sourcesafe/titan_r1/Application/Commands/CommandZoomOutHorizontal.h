/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_ZOOM_OUT_HORIZONTAL
#define __COMMAND_ZOOM_OUT_HORIZONTAL

#include "EMCommand.h"

class TrackWindow;

class CommandZoomOutHorizontal : public EMCommand
{
public:
	CommandZoomOutHorizontal();
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();
};

#endif
