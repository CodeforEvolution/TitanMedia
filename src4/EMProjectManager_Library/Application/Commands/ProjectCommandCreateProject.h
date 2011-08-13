/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __PROJECT_COMMAND_CREATE_PROJECT
#define __PROJECT_COMMAND_CREATE_PROJECT

#include "EMCommand.h"

class ProjectCommandCreateProject : public EMCommand
{
public:
	ProjectCommandCreateProject();
	void ExecuteE() { };
	void* ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree);
	bool IsUndoable();
	bool RequiresParameters();
};

#endif
