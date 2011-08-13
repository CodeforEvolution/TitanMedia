/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_QUANTIZE_MIDI_EVENTS
#define __MEDIA_COMMAND_QUANTIZE_MIDI_EVENTS

#include "EMCommand.h"
#include "EMMediaMIDIEvent.h"

#include <map>

class MediaCommandQuantizeMIDIEvents : public EMCommand
{
public:
	MediaCommandQuantizeMIDIEvents(const map<EMMediaMIDIEvent*, int64>* p_opUndoInformation = NULL);
	void ExecuteE() {};
	void* ExecuteE(void* p_upEventList, void*, void*);
	bool RequiresParameters();
	void UndoE();
	bool IsUndoable();
	EMCommand* CloneAndClear();
	
private:
	map<EMMediaMIDIEvent*, int64> m_oUndoInformation;
};

#endif
