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

#ifndef __COMMAND_DISPLAY_CHANCE_CLIP_NAME_DIALOG
#define __COMMAND_DISPLAY_CHANCE_CLIP_NAME_DIALOG

#include "EMCommand.h"
#include "ChangeClipNameDialog.h"

class TrackView;

class CommandDisplayChangeClipNameDialog : public EMCommand
{
public:
	CommandDisplayChangeClipNameDialog(TrackView* p_opTrackView);
	void ExecuteE(){};
	void* ExecuteE(void*, void*, void* );
	bool IsUndoable();
	bool RequiresParameters();

private:
	ChangeClipNameDialog m_oDialog;
	TrackView* m_opTrackView;
	bool m_vInitialized;
};

#endif
