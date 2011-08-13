/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_ACTIVE_MIDI_TRACK_STEP_DOWN
#define __COMMAND_ACTIVE_MIDI_TRACK_STEP_DOWN

#include "EMCommand.h"

class EMGTrack;
class TrackInfo;

class CommandActiveMIDITrackStepDown : public EMCommand
{
public:
	CommandActiveMIDITrackStepDown(TrackInfo* p_opTrackInfo);
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();

private:
	TrackInfo* m_opTrackInfo;
};

#endif

