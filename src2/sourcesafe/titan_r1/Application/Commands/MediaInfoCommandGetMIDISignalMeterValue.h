/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_MIDI_SIGNAL_METER_VALUE
#define __MEDIA_INFO_COMMAND_GET_MIDI_SIGNAL_METER_VALUE

#include "EMCommand.h"

class MediaInfoCommandGetMIDISignalMeterValue : public EMCommand
{
public:
	MediaInfoCommandGetMIDISignalMeterValue();
	~MediaInfoCommandGetMIDISignalMeterValue();
	void ExecuteE() {};
	void* ExecuteE(void* p_upList, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();
	
private:
	list<float>* m_opZeroValues;
};

#endif
