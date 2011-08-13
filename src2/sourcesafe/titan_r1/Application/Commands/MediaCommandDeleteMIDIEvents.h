/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_DELETE_MIDI_EVENTS
#define __MEDIA_COMMAND_DELETE_MIDI_EVENTS

#include "EMCommand.h"
#include "EMMediaMIDIEvent.h"

class MediaCommandDeleteMIDIEvents : public EMCommand
{
public:
	MediaCommandDeleteMIDIEvents();
	MediaCommandDeleteMIDIEvents(list<EMMediaMIDIEvent*>* p_opDeletedEvents);
	~MediaCommandDeleteMIDIEvents();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMIDIEventList, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();
	void UndoE();
	EMCommand* CloneAndClear();
private:
	list<EMMediaMIDIEvent*> m_oDeletedEvents;
	list<int32> m_oReturnValue;
};

#endif
