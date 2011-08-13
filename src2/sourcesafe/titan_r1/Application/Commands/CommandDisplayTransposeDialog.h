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

#ifndef __COMMAND_DISPLAY_TRANSPOSE_DIALOG
#define __COMMAND_DISPLAY_TRANSPOSE_DIALOG

#include "EMCommand.h"
#include "TransposeDialog.h"

class CommandDisplayTransposeDialog : public EMCommand
{
public:
	CommandDisplayTransposeDialog();
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();

private:
	TransposeDialog m_oDialog;
	bool m_vInitialized;
};

#endif
