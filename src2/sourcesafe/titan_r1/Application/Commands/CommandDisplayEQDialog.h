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

#ifndef __COMMAND_DISPLAY_EQ_DIALOG
#define __COMMAND_DISPLAY_EQ_DIALOG

#include "EMCommand.h"
#include "EQDialog.h"

class CommandDisplayEQDialog : public EMCommand
{
public:
	CommandDisplayEQDialog(EMWindow* p_opWindow);
	void ExecuteE(){};
	void* ExecuteE(void* p_upEQ, void* p_upParameterTwo, void* p_upParameterThree);
	bool IsUndoable();
	bool RequiresParameters();

private:
	EQDialog m_oDialog;
	EMWindow* m_opWindow;
	bool m_vInitialized;
};

#endif