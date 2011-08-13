/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_SET_CLIP_EDIT
#define __COMMAND_SET_CLIP_EDIT

#include "EMCommand.h"

class CommandSetClipEdit : public EMCommand
{
public:
	CommandSetClipEdit();
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();
};

#endif
