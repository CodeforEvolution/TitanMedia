/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_SET_FX_EDIT
#define __COMMAND_SET_FX_EDIT

#include "EMCommand.h"

class CommandSetFXEdit : public EMCommand
{
public:
	CommandSetFXEdit();
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();
};

#endif
