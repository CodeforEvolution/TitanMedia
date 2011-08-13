/*******************************************************
* This code is the copyrighted property of Elegant Media
*-------------------------------------------------------
* Creator: Johan Dunberg
* Component: Command
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/
#include "EMGlobals.h"

#ifndef __COMMAND_DISPLAY_PROJECT_SETTINGS_DIALOG
#define __COMMAND_DISPLAY_PROJECT_SETTINGS_DIALOG

#include "EMCommand.h"
#include "ProjectSettingsDialog.h"

class EMWindow;

class CommandDisplayProjectSettingsDialog : public EMCommand
{
public:
	CommandDisplayProjectSettingsDialog(EMWindow* p_opWindow);
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();

private:
	ProjectSettingsDialog m_oDialog;
	EMWindow* m_opParentWindow;
	bool m_vInitialized;
};

#endif
