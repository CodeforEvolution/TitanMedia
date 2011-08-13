#ifndef __EM_MEDIA_MIDI_EVENT_LISTENER
#define __EM_MEDIA_MIDI_EVENT_LISTENER

#include "EMListener.h"
#include "EMListenerRepository.h"

class EMRealtimeMIDIInputDescriptor;

class EMMediaMIDIEventListener : public EMListener
{
public:
	virtual ~EMMediaMIDIEventListener();
	virtual bool MIDIEventReceived(EMRealtimeMIDIInputDescriptor* p_opReceiver, uint64 p_vSngTmEventTime, uint64 p_vLengthBytes, void* p_vpData) = 0;
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);

protected:
	EMMediaMIDIEventListener();
};

#endif