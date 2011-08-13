/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SET_TRACK_MIDI_CHANNEL
#define __MEDIA_COMMAND_SET_TRACK_MIDI_CHANNEL

#include "EMCommand.h"
class EMMediaTrack;
class EMBeRealtimeInputDescriptor;

class MediaCommandSetTrackMIDIChannel : public EMCommand
{
public:
	MediaCommandSetTrackMIDIChannel(int32 p_vRecentTrackID = -1, int32 p_vRecentMIDIChannel = -1);
	void ExecuteE() {};
	void* ExecuteE(void* p_upTrackID, void* p_upMIDIChannel, void*);
	bool RequiresParameters();
	void UndoE();
	bool IsUndoable();
	EMCommand* CloneAndClear();
	
private:
	int32 m_vRecentTrackID;
	int32 m_vRecentMIDIChannel;
};

#endif
