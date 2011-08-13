#include "MediaCommandCreateMIDITrack.h"

#include <windows.h>

MediaCommandCreateMIDITrack::MediaCommandCreateMIDITrack()
{
//	MessageBox(NULL, "MediaCommandCreateMIDITrack created!", "", MB_OK);
}

void MediaCommandCreateMIDITrack::ExecuteE()
{
	ExecuteCreateTrack(EM_TYPE_MIDI, "Untitled MIDI track");
}

void* MediaCommandCreateMIDITrack::ExecuteE(void* p_upTheNameOfTheTrack, void*, void*)
{
	const char* vpName = static_cast<const char*>(p_upTheNameOfTheTrack);
	uint32 vPrio = 0; 
	return ExecuteCreateTrackP(EM_TYPE_MIDI, vpName, vPrio);
}
