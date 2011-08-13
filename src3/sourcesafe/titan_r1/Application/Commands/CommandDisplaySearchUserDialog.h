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

#ifndef __COMMAND_DISPLAY_SEARCH_USER_DIALOG
#define __COMMAND_DISPLAY_SEARCH_USER_DIALOG

#include "EMCommand.h"
#include "SearchUserDialog.h"

class CommandDisplaySearchUserDialog : public EMCommand
{
public:
	CommandDisplaySearchUserDialog(EMWindow* p_opWindow);
	void ExecuteE(){}
	void* ExecuteE(void* p_opParentWindow, void* p_opSystemSettingsDialog, void* );
	bool IsUndoable();
	bool RequiresParameters();

private:
	SearchUserDialog m_oDialog;
	EMWindow* m_opMainWindow;
	bool m_vInitialized;
};

#endif
