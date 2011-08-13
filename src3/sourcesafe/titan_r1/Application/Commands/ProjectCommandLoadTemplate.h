/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __PROJECT_COMMAND_LOAD_TEMPLATE
#define __PROJECT_COMMAND_LOAD_TEMPLATE

#include "EMCommand.h"

class ProjectCommandLoadTemplate : public EMCommand
{
public:
	ProjectCommandLoadTemplate();
	void ExecuteE() { };
	void* ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree);
	bool IsUndoable();
	bool RequiresParameters();
};

#endif
