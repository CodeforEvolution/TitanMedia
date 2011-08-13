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

#ifndef __COMMAND_DISPLAY_HSV_VALUE_DIALOG
#define __COMMAND_DISPLAY_HSV_VALUE_DIALOG

#include "EMCommand.h"
#include "HSV_ValueDialog.h"

class CommandDisplayHSV_ValueDialog : public EMCommand
{
public:
	CommandDisplayHSV_ValueDialog(EMWindow* p_opWindow);
	void ExecuteE(){};
	void* ExecuteE(void* p_opHSVFilter, void*, void* );
	bool IsUndoable();
	bool RequiresParameters();

private:
	HSV_ValueDialog m_oDialog;
	EMWindow* m_opWindow;
	bool m_vInitialized;
};

#endif
