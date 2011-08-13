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

#ifndef __COMMAND_DISPLAY_SEARCH_ADD_USER_DIALOG
#define __COMMAND_DISPLAY_SEARCH_ADD_USER_DIALOG

#include "EMCommand.h"
#include "SearchAddUserDialog.h"

class CommandDisplaySearchAddUserDialog : public EMCommand
{
public:
	CommandDisplaySearchAddUserDialog(EMWindow* p_opWindow);
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();

private:
	SearchAddUserDialog m_oDialog;
	EMWindow* m_opWindow;
	bool m_vInitialized;
};

#endif
