/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_MOVE_MIDI_EVENTS
#define __MEDIA_COMMAND_MOVE_MIDI_EVENTS

#include "EMCommand.h"
#include "EMMediaMIDIEvent.h"

class MediaCommandMoveMIDIEvents : public EMCommand
{
public:
	MediaCommandMoveMIDIEvents();
	MediaCommandMoveMIDIEvents(list<EMMediaMIDIEvent*>* p_opLastEvents, int64 p_vLastOffset);
	~MediaCommandMoveMIDIEvents();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMIDIEventList, void* p_upTimeOffsetInMicroSeconds, void*);
	bool RequiresParameters();
	bool IsUndoable();
	void UndoE();
	EMCommand* CloneAndClear();
private:
	list<EMMediaMIDIEvent*> m_oLastEvents;
	int64 m_vLastOffset;
};

#endif
