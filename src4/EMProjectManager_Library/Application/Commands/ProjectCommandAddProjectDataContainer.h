/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __PROJECT_COMMAND_ADD_PROJECT_DATA_CONTAINER
#define __PROJECT_COMMAND_ADD_PROJECT_DATA_CONTAINER

#include "EMCommand.h"

class ProjectCommandAddProjectDataContainer : public EMCommand
{
public:
	ProjectCommandAddProjectDataContainer();
	void ExecuteE() { };
	void* ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree);
	bool IsUndoable();
	bool RequiresParameters();
};

#endif
