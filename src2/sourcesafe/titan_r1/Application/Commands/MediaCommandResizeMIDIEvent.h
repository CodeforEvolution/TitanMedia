/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_RESIZE_MIDI_EVENT
#define __MEDIA_COMMAND_RESIZE_MIDI_EVENT

#include "EMCommand.h"
class EMMediaMIDIEvent;

class MediaCommandResizeMIDIEvent : public EMCommand
{
public:
	MediaCommandResizeMIDIEvent();
	MediaCommandResizeMIDIEvent(EMMediaMIDIEvent* p_opOldNoteOn, int64 p_vOldStart, int64 p_vOldDuration);
	~MediaCommandResizeMIDIEvent();
	void ExecuteE() {};
	void* ExecuteE(void* p_upNoteOnEvent, void* p_upStart, void* p_upDuration);
	bool RequiresParameters();
	bool IsUndoable();
	void UndoE();
	EMCommand* CloneAndClear();

private:
	EMMediaMIDIEvent* m_opOnEvent;
	int64 m_vOldStart;
	int64 m_vOldDuration;
};

#endif
