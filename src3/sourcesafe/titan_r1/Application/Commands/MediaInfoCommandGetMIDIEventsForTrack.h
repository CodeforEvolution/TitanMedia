/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_MIDI_EVENTS_FOR_TRACK
#define __MEDIA_INFO_COMMAND_GET_MIDI_EVENTS_FOR_TRACK

#include "EMCommand.h"

class EMMediaMIDIEvent;
class EMMediaMIDIEventList;

class MediaInfoCommandGetMIDIEventsForTrack : public EMCommand
{
public:
	MediaInfoCommandGetMIDIEventsForTrack();
	~MediaInfoCommandGetMIDIEventsForTrack();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaTrackID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();
	
private:
	list<EMMediaMIDIEvent*> m_oReturnValue;
	EMMediaMIDIEventList* m_opSortedEventList;
};

#endif
