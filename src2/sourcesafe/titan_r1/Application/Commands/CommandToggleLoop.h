/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_TOGGLE_LOOP
#define __COMMAND_TOGGLE_LOOP

#include "EMCommand.h"

class TrackWindowToolBar;

class CommandToggleLoop : public EMCommand
{
public:
	CommandToggleLoop(TrackWindowToolBar* p_opToolBar);
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();

private:
	TrackWindowToolBar* m_opToolBar;
};

#endif
