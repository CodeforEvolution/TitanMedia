/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_DISPLAY_RENDER_FORMAT_DIALOG
#define __COMMAND_DISPLAY_RENDER_FORMAT_DIALOG

#include "EMCommand.h"
#include "RenderFormatDialog.h"

class CommandDisplayRenderFormatDialog : public EMCommand
{
public:
	CommandDisplayRenderFormatDialog();
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();

private:
	RenderFormatDialog m_oDialog;
//	bool m_vInitialized;
};

#endif
