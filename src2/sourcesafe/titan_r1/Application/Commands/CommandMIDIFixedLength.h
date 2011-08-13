/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_MIDI_FIXED_LENGTH
#define __COMMAND_MIDI_FIXED_LENGTH

#include "EMCommand.h"

class TrackWindow;

class CommandMIDIFixedLength : public EMCommand
{
public:
	CommandMIDIFixedLength(TrackWindow* p_opTrackWindow);
	void ExecuteE();
	bool IsUndoable();
	bool RequiresParameters();

private:
	TrackWindow* m_opTrackWindow;
};

#endif
