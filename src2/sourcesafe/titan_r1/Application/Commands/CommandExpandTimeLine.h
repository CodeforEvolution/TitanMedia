/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_EXPAND_TIMELINE
#define __COMMAND_EXPAND_TIMELINE

#include "EMCommand.h"

class TrackWindow;

class CommandExpandTimeLine : public EMCommand
{
public:
	CommandExpandTimeLine(TrackWindow* p_opTrackWindow);
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();

private:
	TrackWindow* m_opTrackWindow;
};

#endif


