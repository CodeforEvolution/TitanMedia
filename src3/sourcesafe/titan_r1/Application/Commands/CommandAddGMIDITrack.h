/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_ADD_G_MIDI_TRACK
#define __COMMAND_ADD_G_MIDI_TRACK

#include "EMCommand.h"

class EMGTrack;
class TrackInfo;
class TrackView;

class CommandAddGMIDITrack : public EMCommand
{
public:
	CommandAddGMIDITrack(TrackView* p_opTrackView, TrackInfo* p_opTrackInfo);
	~CommandAddGMIDITrack();
	EMCommand* CloneAndClear();
	void ExecuteE() {};
	void* ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree);
	const char* GetShortDescription();
	bool IsUndoable();
	bool RequiresParameters();
	void SetTrack(EMGTrack* p_opTrack);
	void UndoE();

private:
	EMGTrack* m_opTrack;
	TrackInfo* m_opTrackInfo;
	TrackView* m_opTrackView;
};

#endif

