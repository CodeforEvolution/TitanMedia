/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_STOP
#define __COMMAND_STOP

#include "EMCommand.h"

class TrackWindowToolBar;

class CommandStop : public EMCommand
{
public:
	CommandStop(TrackWindowToolBar* p_opToolBar);
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();

private:
	TrackWindowToolBar* m_opToolBar;
};

#endif
