/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_CREATE_PROJECT
#define __COMMAND_CREATE_PROJECT

#include "EMCommand.h"

class FileDialogTarget;

class CommandCreateProject : public EMCommand
{
public:
	CommandCreateProject(FileDialogTarget* p_opFileDialogTarget);
	void ExecuteE() {};
	void* ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree);
	bool IsUndoable();
	bool RequiresParameters();

private:
	FileDialogTarget* m_opFileDialogTarget;	
};

#endif
