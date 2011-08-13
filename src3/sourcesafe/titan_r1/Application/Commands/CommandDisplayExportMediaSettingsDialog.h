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

#ifndef __COMMAND_DISPLAY_EXPORT_MEDIA_SETTINGS_DIALOG
#define __COMMAND_DISPLAY_EXPORT_MEDIA_SETTINGS_DIALOG

#include "EMCommand.h"
#include "ExportAudioSettingsDialog.h"
#include "ExportMediaSettingsDialog.h"	

class CommandDisplayExportMediaSettingsDialog : public EMCommand
{
public:
	CommandDisplayExportMediaSettingsDialog(EMWindow* p_opWindow);
	void ExecuteE(){};
	void* ExecuteE(void*, void*, void* );// { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();

private:
	ExportAudioSettingsDialog m_oAudioDialog;
	ExportMediaSettingsDialog m_oMediaDialog;
	EMWindow* m_opWindow;
	bool m_vInitialized;
};

#endif
