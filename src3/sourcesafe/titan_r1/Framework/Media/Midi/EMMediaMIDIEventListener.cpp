#include "EMMediaMIDIEventListener.h"

//#include "EMMIDIGlobals.h"
#include "EMWinDXMIDIConsumer.h"

#include "EMRealtimeMIDIInputDescriptor.h"

EMMediaMIDIEventListener::EMMediaMIDIEventListener()
{
}

EMMediaMIDIEventListener::~EMMediaMIDIEventListener()
{
}

bool EMMediaMIDIEventListener::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
/*	switch(p_vMessage)
	{
		case EM_MESSAGE_MIDI_EVENT_RECEIVED:
		{
			EMWinDXMIDIConsumer* opConsumer = static_cast<EMWinDXMIDIConsumer*>(p_opSender);

			EMRealtimeMIDIInputDescriptor* opReceiver = NULL;
			uint64 vSongTime = 0;
			uint64 vLength = 0;
			void* vpData = NULL;
 
			opConsumer -> GetLastMIDIEvent(&opReceiver, &vSongTime, &vLength, &vpData);
			return MIDIEventReceived(opReceiver, vSongTime, vLength, vpData);
		}
		default:
			return false;
	}; */
	return true;
}
