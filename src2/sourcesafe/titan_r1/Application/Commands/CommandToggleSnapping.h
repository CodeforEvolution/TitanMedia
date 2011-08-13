/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_TOGGLE_SNAPPING
#define __COMMAND_TOGGLE_SNAPPING

#include "EMCommand.h"

class CommandToggleSnapping : public EMCommand
{
public:
	CommandToggleSnapping();
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();
};

#endif
