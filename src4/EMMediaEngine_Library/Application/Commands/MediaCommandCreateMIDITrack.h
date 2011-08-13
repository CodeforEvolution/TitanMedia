/*******************************************************
* Creator: Jesper Svensson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/
#include "MediaCommandCreateTrack.h"

#ifndef __MEDIA_COMMAND_CREATE_MIDI_TRACK
#define __MEDIA_COMMAND_CREATE_MIDI_TRACK

class MediaCommandCreateMIDITrack : public MediaCommandCreateTrack
{
public:
	void ExecuteE();
	void* ExecuteE(void* p_upTheNameOfTheTrack, void*, void*);
	MediaCommandCreateMIDITrack();

private:

};

#endif
