/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_ADD_MIDI_TRACK
#define __COMMAND_ADD_MIDI_TRACK

#include "EMCommand.h"

class EMGTrack;
class TrackInfo;
class TrackView;

class CommandAddMIDITrack : public EMCommand
{
public:
	CommandAddMIDITrack(TrackView* p_opTrackView, TrackInfo* p_opTrackInfo);
	~CommandAddMIDITrack();
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();
	void SetTrack(EMGTrack* p_opTrack);

private:
	EMGTrack* m_opTrack;
	TrackInfo* m_opTrackInfo;
	TrackView* m_opTrackView;
};

#endif

