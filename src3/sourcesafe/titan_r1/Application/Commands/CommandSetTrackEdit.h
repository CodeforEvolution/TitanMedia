/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_SET_TRACK_EDIT
#define __COMMAND_SET_TRACK_EDIT

#include "EMCommand.h"

class CommandSetTrackEdit : public EMCommand
{
public:
	CommandSetTrackEdit();
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();
};

#endif
