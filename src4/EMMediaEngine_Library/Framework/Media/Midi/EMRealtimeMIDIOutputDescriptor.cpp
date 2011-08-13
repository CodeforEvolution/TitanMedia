#include "EMRealtimeMIDIOutputDescriptor.h"

#include "EMMediaTimer.h"

EMRealtimeMIDIOutputDescriptor::EMRealtimeMIDIOutputDescriptor()
	:	EMRealtimeOutputDescriptor(EM_TYPE_MIDI)
{
//	EMMediaTimer::Instance() -> AddListener(this);
}

EMRealtimeMIDIOutputDescriptor::~EMRealtimeMIDIOutputDescriptor()
{
//	EMMediaTimer::Instance() -> RemoveListener(this);
} 