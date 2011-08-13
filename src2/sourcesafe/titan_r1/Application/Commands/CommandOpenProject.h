/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_OPEN_PROJECT
#define __COMMAND_OPEN_PROJECT

#include "EMCommand.h"

class FileDialogTarget;

class CommandOpenProject : public EMCommand
{
public:
	CommandOpenProject(FileDialogTarget* p_opFileDialogTarget);
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();

private:
	FileDialogTarget* m_opFileDialogTarget;	
};

#endif
