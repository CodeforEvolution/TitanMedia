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

#ifndef __COMMAND_DISPLAY_SET_VELOCITY_DIALOG
#define __COMMAND_DISPLAY_SET_VELOCITY_DIALOG

#include "EMCommand.h"
#include "SetVelocityDialog.h"

class CommandDisplaySetVelocityDialog : public EMCommand
{
public:
	CommandDisplaySetVelocityDialog();
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();

private:
	SetVelocityDialog m_oDialog;
	bool m_vInitialized;
};

#endif
