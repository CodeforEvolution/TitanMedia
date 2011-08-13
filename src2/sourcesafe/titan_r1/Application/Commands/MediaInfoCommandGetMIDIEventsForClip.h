/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_MIDI_EVENTS_FOR_CLIP
#define __MEDIA_INFO_COMMAND_GET_MIDI_EVENTS_FOR_CLIP

#include "EMCommand.h"

class EMMediaMIDIEvent;

class MediaInfoCommandGetMIDIEventsForClip : public EMCommand
{
public:
	MediaInfoCommandGetMIDIEventsForClip();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaClipID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();
	
private:
	list<EMMediaMIDIEvent*> m_oReturnValue;
};

#endif
