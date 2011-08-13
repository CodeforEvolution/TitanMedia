/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __PROJECT_COMMAND_CLOSE_PROJECT
#define __PROJECT_COMMAND_CLOSE_PROJECT

#include "EMCommand.h"

class ProjectCommandCloseProject : public EMCommand
{
public:
	ProjectCommandCloseProject();
	void ExecuteE() { };
	void* ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree);
	bool IsUndoable();
	bool RequiresParameters();
};

#endif
