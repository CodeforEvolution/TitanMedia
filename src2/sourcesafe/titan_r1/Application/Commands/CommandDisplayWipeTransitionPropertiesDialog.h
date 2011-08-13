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

#ifndef __COMMAND_DISPLAY_WIPE_TRANSITION_PROPERTIES_DIALOG
#define __COMMAND_DISPLAY_WIPE_TRANSITION_PROPERTIES_DIALOG

#include "EMCommand.h"
#include "WipeTransitionPropertiesDialog.h"	

class CommandDisplayWipeTransitionPropertiesDialog : public EMCommand
{
public:
	CommandDisplayWipeTransitionPropertiesDialog(EMWindow* p_opWindow);
	void ExecuteE(){};
	void* ExecuteE(void*, void*, void* );// { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();

private:
	WipeTransitionPropertiesDialog m_oDialog;
	EMWindow* m_opWindow;
	bool m_vInitialized;
};

#endif
