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

#ifndef __COMMAND_DISPLAY_COLOR_BALANCE_DIALOG
#define __COMMAND_DISPLAY_COLOR_BALANCE_DIALOG

#include "EMCommand.h"
#include "ColorBalanceDialog.h"

class CommandDisplayColorBalanceDialog : public EMCommand
{
public:
	CommandDisplayColorBalanceDialog(EMWindow* p_opWindow);
	void ExecuteE(){};
	void* ExecuteE(void* p_opRGBFilter, void*, void* );
	bool IsUndoable();
	bool RequiresParameters();

private:
	ColorBalanceDialog m_oDialog;
	EMWindow* m_opWindow;
	bool m_vInitialized;
};

#endif
