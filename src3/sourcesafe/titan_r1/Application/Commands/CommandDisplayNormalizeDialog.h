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

#ifndef __COMMAND_DISPLAY_NORMALIZE_DIALOG
#define __COMMAND_DISPLAY_NORMALIZE_DIALOG

#include "EMCommand.h"
#include "NormalizeDialog.h"

class CommandDisplayNormalizeDialog : public EMCommand
{
public:
	CommandDisplayNormalizeDialog(EMWindow* p_opWindow);
	void ExecuteE(){};
	void* ExecuteE(void* p_opCommandAudioNormalize, void* p_upParameterTwo, void* p_upParameterThree);
	bool IsUndoable();
	bool RequiresParameters();

private:
	NormalizeDialog m_oDialog;
	EMWindow* m_opWindow;
	bool m_vInitialized;
};

#endif
