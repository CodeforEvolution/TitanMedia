/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_QUIT_APPLICATION
#define __COMMAND_QUIT_APPLICATION

#include "EMCommand.h"

class EMMessageBox;
class TrackWindow;

class CommandQuitApplication : public EMCommand
{
public:
	CommandQuitApplication(TrackWindow* p_opTrackWindow);
	void ExecuteE();
	bool IsUndoable();
	bool RequiresParameters();

private:
	TrackWindow* m_opTrackWindow;
	EMMessageBox* m_opMsgBox;
};

#endif
