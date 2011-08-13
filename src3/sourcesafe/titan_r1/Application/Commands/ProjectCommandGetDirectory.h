/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __PROJECT_COMMAND_GET_DIRECTORY
#define __PROJECT_COMMAND_GET_DIRECTORY

#include "EMCommand.h"

#include <string>

class ProjectCommandGetDirectory : public EMCommand
{
public:
	ProjectCommandGetDirectory();
	void ExecuteE() { };
	void* ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree);
	bool IsUndoable();
	bool RequiresParameters();

private:
	string m_oReturnValue; // The requested directory
};

#endif
