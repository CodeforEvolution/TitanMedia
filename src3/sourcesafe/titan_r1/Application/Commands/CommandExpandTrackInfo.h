/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_EXPAND_TRACK_INFO
#define __COMMAND_EXPAND_TRACK_INFO

#include "EMCommand.h"

class TrackWindow;

class CommandExpandTrackInfo : public EMCommand
{
public:
	CommandExpandTrackInfo(TrackWindow* p_opTrackWindow);
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();

private:
	TrackWindow* m_opTrackWindow;
};

#endif
