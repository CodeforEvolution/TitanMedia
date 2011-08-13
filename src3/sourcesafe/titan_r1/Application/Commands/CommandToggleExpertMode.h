/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_TOGGLE_EXPERT_MODE
#define __COMMAND_TOGGLE_EXPERT_MODE

#include "EMCommand.h"

class EMMenuBar;
class TrackView;

class CommandToggleExpertMode : public EMCommand
{
public:
	CommandToggleExpertMode(EMMenuBar* p_opMenuBar, TrackView* p_opTrackView);
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();

private:
	EMMenuBar* m_opMenuBar;
	TrackView* m_opTrackView;
};

#endif
