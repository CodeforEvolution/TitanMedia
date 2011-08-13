/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_MIDI_TOGGLE_METRONOME
#define __COMMAND_MIDI_TOGGLE_METRONOME

#include "EMCommand.h"

class EMMenuBar;

class CommandMIDIToggleMetronome : public EMCommand
{
public:
	CommandMIDIToggleMetronome(EMMenuBar* p_opMenuBar);
	void ExecuteE();
	bool IsUndoable();
	bool RequiresParameters();

private:
	EMMenuBar* m_opMenuBar;
};

#endif
