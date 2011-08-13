/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_DISPLAY_NEW_PROJECT_DIALOG
#define __COMMAND_DISPLAY_NEW_PROJECT_DIALOG

#include "EMCommand.h"
#include "NewProjectDialog.h"

class EMWindow;

class CommandDisplayNewProjectDialog : public EMCommand
{
public:
	CommandDisplayNewProjectDialog(EMWindow* p_opWindow);
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();

private:
	NewProjectDialog m_oDialog;
	EMWindow* m_opWindow;
	bool m_vInitialized;
};

#endif
