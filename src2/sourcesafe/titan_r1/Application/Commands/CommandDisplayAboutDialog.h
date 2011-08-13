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

#ifndef __COMMAND_DISPLAY_ABOUT_DIALOG
#define __COMMAND_DISPLAY_ABOUT_DIALOG

#include "EMCommand.h"
#include "AboutDialog.h"

class CommandDisplayAboutDialog : public EMCommand
{
public:
	CommandDisplayAboutDialog(EMWindow* p_opWindow);
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ){return NULL;};
	bool IsUndoable();
	bool RequiresParameters();

private:
	AboutDialog m_oDialog;
	EMWindow* m_opWindow;
	bool m_vInitialized;
};

#endif
