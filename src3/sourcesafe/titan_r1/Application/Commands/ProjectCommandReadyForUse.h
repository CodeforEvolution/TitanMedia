/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __PROJECT_COMMAND_READY_FOR_USE
#define __PROJECT_COMMAND_READY_FOR_USE

#include "EMCommand.h"

class ProjectCommandReadyForUse : public EMCommand
{
public:
	ProjectCommandReadyForUse();
	void ExecuteE();
	void* ExecuteE(void*, void*, void*) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();
};

#endif
