/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SEND_MIDI_EVENT
#define __MEDIA_COMMAND_SEND_MIDI_EVENT

#include "EMCommand.h"
class EMMediaMIDIEvent;

class MediaCommandSendMIDIEvent : public EMCommand
{
public:
	MediaCommandSendMIDIEvent();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaTrackID, void* p_upKeyNumber, void* p_upVelocity);
	bool RequiresParameters();
	bool IsUndoable();

private:
};

#endif
