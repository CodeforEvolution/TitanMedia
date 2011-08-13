/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_TOGGLE_AUTO_QUANTIZE
#define __COMMAND_TOGGLE_AUTO_QUANTIZE

#include "EMCommand.h"

class CommandToggleAutoQuantize : public EMCommand
{
public:
	CommandToggleAutoQuantize();
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();
};

#endif
