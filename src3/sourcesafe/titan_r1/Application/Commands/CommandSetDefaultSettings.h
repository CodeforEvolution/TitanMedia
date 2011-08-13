/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_SET_DEFAULT_SETTINGS
#define __COMMAND_SET_DEFAULT_SETTINGS

#include "EMCommand.h"

class CommandSetDefaultSettings : public EMCommand
{
public:
	CommandSetDefaultSettings();
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();
};

#endif
