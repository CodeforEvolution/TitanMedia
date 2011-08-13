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

#ifndef __COMMAND_DISPLAY_TRANSITION_DIALOG
#define __COMMAND_DISPLAY_TRANSITION_DIALOG

#include "EMCommand.h"
#include "TransitionDialog.h"

class CommandDisplayTransitionDialog : public EMCommand
{
public:
	CommandDisplayTransitionDialog(EMWindow* p_opWindow, EMView* p_opView);
	void ExecuteE(){};
	void* ExecuteE(void*, void*, void* );
	bool IsUndoable();
	bool RequiresParameters();

private:
	TransitionDialog m_oDialog;
	EMView* m_opView;
	EMWindow* m_opWindow;
	bool m_vInitialized;
};

#endif
