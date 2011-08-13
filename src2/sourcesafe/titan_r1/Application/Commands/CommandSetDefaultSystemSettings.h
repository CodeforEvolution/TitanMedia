/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_SET_DEFAULT_SYSTEM_SETTINGS
#define __COMMAND_SET_DEFAULT_SYSTEM_SETTINGS

#include "EMCommand.h"

class CommandSetDefaultSystemSettings : public EMCommand
{
public:
	CommandSetDefaultSystemSettings();
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();
};

#endif
