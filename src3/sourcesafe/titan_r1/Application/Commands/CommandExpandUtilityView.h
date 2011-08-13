/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_EXPAND_UTILITY_VIEW
#define __COMMAND_EXPAND_UTILITY_VIEW

#include "EMCommand.h"

class TrackWindow;

class CommandExpandUtilityView : public EMCommand
{
public:
	CommandExpandUtilityView(TrackWindow* p_opTrackWindow);
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();

private:
	TrackWindow* m_opTrackWindow;
};

#endif



