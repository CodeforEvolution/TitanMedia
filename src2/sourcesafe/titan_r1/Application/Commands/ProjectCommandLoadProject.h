/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __PROJECT_COMMAND_LOAD_PROJECT
#define __PROJECT_COMMAND_LOAD_PROJECT

#include "EMCommand.h"

class ProjectCommandLoadProject : public EMCommand
{
public:
	ProjectCommandLoadProject();
	void ExecuteE() { };
	void* ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree);
	bool IsUndoable();
	bool RequiresParameters();
};

#endif
