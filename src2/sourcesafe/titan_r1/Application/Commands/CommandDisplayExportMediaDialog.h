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

#ifndef __COMMAND_DISPLAY_EXPORT_MEDIA_DIALOG
#define __COMMAND_DISPLAY_EXPORT_MEDIA_DIALOG

#include "EMCommand.h"
#include "ExportMediaDialog.h"

class CommandDisplayExportMediaDialog : public EMCommand
{
public:
	CommandDisplayExportMediaDialog(EMWindow* p_opWindow);
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();

private:
	ExportMediaDialog m_oDialog;
	EMWindow* m_opWindow;
	bool m_vInitialized;
};

#endif
