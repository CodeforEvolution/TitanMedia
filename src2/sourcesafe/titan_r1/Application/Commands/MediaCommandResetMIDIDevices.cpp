#include "MediaCommandResetMIDIDevices.h"

#include "EMMIDIGlobals.h"
#include "EMWinDXMIDIProducer.h"

const struct EMStandardMIDIMessage
{
	uint8 m_vStatusByte;
	uint8 m_vSecondByte;
	uint8 m_vThirdByte;
	uint8 m_vLength;
};

#define LengthOf(l) (l[3])
const char MIDI_MESSAGE_SYSTEM_RESET[4] = {(char) EM_MIDI_EVENT_TYPE_SYSTEM_RESET, 0, 0, 1};
const char MIDI_MESSAGE_ALL_NOTES_OFF[4] = {(char) EM_MIDI_EVENT_TYPE_CONTROL_CHANGE, (char) EM_MIDI_CONTROL_CHANGE_TYPE_ALL_NOTES_OFF, 0, 3};
const char MIDI_MESSAGE_ALL_SOUND_OFF[4] = {(char) EM_MIDI_EVENT_TYPE_CONTROL_CHANGE, (char) EM_MIDI_CONTROL_CHANGE_TYPE_ALL_SOUND_OFF, 0, 3};
const char MIDI_MESSAGE_RESET_CONTROLLERS[4] = {(char) EM_MIDI_EVENT_TYPE_CONTROL_CHANGE, (char) EM_MIDI_CONTROL_CHANGE_TYPE_RESET_ALL_CONTROLLERS, 0, 3};

MediaCommandResetMIDIDevices::MediaCommandResetMIDIDevices()
{
}

void* MediaCommandResetMIDIDevices::ExecuteE(void*, void*, void*)
{
	uint8 vpStandardMIDIMessage[3];
	
	vpStandardMIDIMessage[0] = EM_MIDI_EVENT_TYPE_SYSTEM_RESET;
	vpStandardMIDIMessage[1] = 0;
	vpStandardMIDIMessage[2] = 0;
	EMWinDXMIDIProducer::Instance() -> BroadcastMIDIMessage(1, vpStandardMIDIMessage); 

	vpStandardMIDIMessage[0] = EM_MIDI_EVENT_TYPE_CONTROL_CHANGE;
	vpStandardMIDIMessage[1] = EM_MIDI_CONTROL_CHANGE_TYPE_ALL_NOTES_OFF;
	vpStandardMIDIMessage[2] = 0;
	EMWinDXMIDIProducer::Instance() -> BroadcastMIDIMessage(3, vpStandardMIDIMessage); 

	vpStandardMIDIMessage[0] = EM_MIDI_EVENT_TYPE_CONTROL_CHANGE;
	vpStandardMIDIMessage[1] = EM_MIDI_CONTROL_CHANGE_TYPE_ALL_SOUND_OFF;
	vpStandardMIDIMessage[2] = 0;
	EMWinDXMIDIProducer::Instance() -> BroadcastMIDIMessage(3, vpStandardMIDIMessage); 

	vpStandardMIDIMessage[0] = EM_MIDI_EVENT_TYPE_CONTROL_CHANGE;
	vpStandardMIDIMessage[1] = EM_MIDI_CONTROL_CHANGE_TYPE_RESET_ALL_CONTROLLERS;
	vpStandardMIDIMessage[2] = 0;
	EMWinDXMIDIProducer::Instance() -> BroadcastMIDIMessage(3, vpStandardMIDIMessage); 
	return NULL;
}

bool MediaCommandResetMIDIDevices::RequiresParameters()
{
	return true;
}

bool MediaCommandResetMIDIDevices::IsUndoable()
{
	return false;
}