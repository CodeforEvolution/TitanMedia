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

#ifndef __COMMAND_DISPLAY_REGISTRATION_DIALOG
#define __COMMAND_DISPLAY_REGISTRATION_DIALOG

#include "EMCommand.h"
#include "RegistrationDialog.h"

class CommandDisplayRegistrationDialog : public EMCommand
{
public:
	CommandDisplayRegistrationDialog(EMWindow* p_opWindow);
	void ExecuteE() {};
	void* ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree);
	bool IsUndoable();
	bool RequiresParameters();

private:
	RegistrationDialog m_oDialog;
	EMWindow* m_opWindow;
	bool m_vInitialized;
};

#endif
