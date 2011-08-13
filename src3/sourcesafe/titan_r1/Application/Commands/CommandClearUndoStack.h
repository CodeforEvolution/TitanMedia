/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_CLEAR_UNDO_STACK
#define __COMMAND_CLEAR_UNDO_STACK

#include "EMCommand.h"

class CommandClearUndoStack : public EMCommand
{
public:
	CommandClearUndoStack();
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();
};

#endif
