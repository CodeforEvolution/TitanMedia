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

#ifndef __COMMAND_DISPLAY_SYSTEM_SETTINGS_DIALOG
#define __COMMAND_DISPLAY_SYSTEM_SETTINGS_DIALOG

#include "EMCommand.h"
#include "SystemSettingsDialog.h"

class EMWindow;

class CommandDisplaySystemSettingsDialog : public EMCommand
{
public:
	CommandDisplaySystemSettingsDialog(EMWindow* p_opWindow);
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();

private:
	SystemSettingsDialog m_oDialog;
	EMWindow* m_opParentWindow;
	bool m_vInitialized;
};

#endif
