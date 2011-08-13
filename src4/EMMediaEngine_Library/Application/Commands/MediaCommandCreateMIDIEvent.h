/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_CREATE_MIDI_EVENT
#define __MEDIA_COMMAND_CREATE_MIDI_EVENT

#include "EMCommand.h"
class EMMediaMIDIEvent;

class MediaCommandCreateMIDIEvent : public EMCommand
{
public:
	MediaCommandCreateMIDIEvent();
	MediaCommandCreateMIDIEvent(list<EMMediaMIDIEvent*>* p_opRecentlyCreatedEvents);
	void ExecuteE() {};
	void* ExecuteE(void* p_upSeries1, void* p_upSeries2, void*);
	bool RequiresParameters();
	bool IsUndoable();
	void UndoE();
	EMCommand* CloneAndClear();
private:
	list<EMMediaMIDIEvent*> m_oRecentlyCreatedEvents;
	list<EMMediaMIDIEvent*> m_oReturnValue;
};

#endif
