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

#ifndef __COMMAND_DISPLAY_QUANTIZE_DIALOG
#define __COMMAND_DISPLAY_QUANTIZE_DIALOG

#include "EMCommand.h"
#include "QuantizeDialog.h"

class CommandDisplayQuantizeDialog : public EMCommand
{
public:
	CommandDisplayQuantizeDialog();
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();

private:
	QuantizeDialog m_oDialog;
	bool m_vInitialized;
};

#endif
