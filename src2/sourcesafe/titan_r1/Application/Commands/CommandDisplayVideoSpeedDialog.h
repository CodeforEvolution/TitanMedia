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

#ifndef __COMMAND_DISPLAY_VIDEO_SPEED_DIALOG
#define __COMMAND_DISPLAY_VIDEO_SPEED_DIALOG

#include "EMCommand.h"
#include "VideoSpeedDialog.h"

class EMWindow;
class TrackView;

class CommandDisplayVideoSpeedDialog : public EMCommand
{
public:
	CommandDisplayVideoSpeedDialog(EMWindow* p_opWindow);
	void ExecuteE();
	void* ExecuteE(void* , void*, void* ){return NULL;};
	bool IsUndoable();
	bool RequiresParameters();

private:
	VideoSpeedDialog m_oDialog;
	EMWindow* m_opParentWindow;
	bool m_vInitialized;
};

#endif
