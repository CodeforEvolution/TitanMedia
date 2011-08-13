/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_ACTIVE_MIDI_TRACK_STEP_UP
#define __COMMAND_ACTIVE_MIDI_TRACK_STEP_UP

#include "EMCommand.h"

class EMGTrack;
class TrackInfo;

class CommandActiveMIDITrackStepUp : public EMCommand
{
public:
	CommandActiveMIDITrackStepUp(TrackInfo* p_opTrackInfo);
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();

private:
	TrackInfo* m_opTrackInfo;
};

#endif

