/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_RESET_MIDI_DEVICES
#define __MEDIA_COMMAND_RESET_MIDI_DEVICES

#include "EMCommand.h"

class MediaCommandResetMIDIDevices : public EMCommand
{
public:
	MediaCommandResetMIDIDevices();
	void ExecuteE() {};
	void* ExecuteE(void*, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
};

#endif
