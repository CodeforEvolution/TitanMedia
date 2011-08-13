/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SET_ACTIVE_MIDI_TRACK
#define __MEDIA_COMMAND_SET_ACTIVE_MIDI_TRACK

#include "EMCommand.h"
class EMMediaTrack;
class EMBeRealtimeInputDescriptor;

class MediaCommandSetActiveMIDITrack : public EMCommand
{
public:
	MediaCommandSetActiveMIDITrack(int32 p_vRecentTrackID = -1);
	void ExecuteE() {};
	void* ExecuteE(void* p_upTrackID, void*, void*);
	bool RequiresParameters();
	void UndoE();
	bool IsUndoable();
	EMCommand* CloneAndClear();
	
private:
	int32 m_vRecentTrackID;
};

#endif
