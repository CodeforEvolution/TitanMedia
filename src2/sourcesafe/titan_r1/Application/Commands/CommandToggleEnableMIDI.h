/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_TOGGLE_ENABLE_MIDI
#define __COMMAND_TOGGLE_ENABLE_MIDI

#include "EMCommand.h"

class EMMenuBar;
class TrackView;

class CommandToggleEnableMIDI : public EMCommand
{
public:
	CommandToggleEnableMIDI(EMMenuBar* p_opMenuBar, TrackView* p_opTrackView);
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();

private:
	EMMenuBar* m_opMenuBar;
	TrackView* m_opTrackView;
};

#endif
