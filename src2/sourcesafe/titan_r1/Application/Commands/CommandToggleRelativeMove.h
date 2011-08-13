/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_TOGGLE_RELATIVE_MOVE
#define __COMMAND_TOGGLE_RELATIVE_MOVE

#include "EMCommand.h"

class CommandToggleRelativeMove : public EMCommand
{
public:
	CommandToggleRelativeMove();
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();
};

#endif
