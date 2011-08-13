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

#ifndef __COMMAND_DISPLAY_VIDEO_RECORDING_DIALOG
#define __COMMAND_DISPLAY_VIDEO_RECORDING_DIALOG

#include "EMCommand.h"
#include "VideoRecordingDialog.h"

class CommandDisplayVideoRecordingDialog : public EMCommand
{
public:
	CommandDisplayVideoRecordingDialog();
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();

private:
	VideoRecordingDialog m_oDialog;
	bool m_vInitialized;
};

#endif
