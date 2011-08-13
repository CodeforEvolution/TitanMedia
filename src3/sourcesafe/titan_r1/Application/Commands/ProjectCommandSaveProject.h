/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __PROJECT_COMMAND_SAVE_PROJECT
#define __PROJECT_COMMAND_SAVE_PROJECT

#include "EMCommand.h"

class ProjectCommandSaveProject : public EMCommand
{
public:
	ProjectCommandSaveProject();
	void ExecuteE() { };
	void* ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree);
	bool IsUndoable();
	bool RequiresParameters();
};

#endif
