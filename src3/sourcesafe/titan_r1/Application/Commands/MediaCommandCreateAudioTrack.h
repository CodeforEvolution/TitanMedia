/*******************************************************
* Creator: Jesper Svensson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/
#include "MediaCommandCreateTrack.h"

#ifndef __MEDIA_COMMAND_CREATE_AUDIO_TRACK
#define __MEDIA_COMMAND_CREATE_AUDIO_TRACK

class MediaCommandCreateAudioTrack : public MediaCommandCreateTrack
{
public:
	void ExecuteE();
	void* ExecuteE(void* p_upTheNameOfTheTrack, void*, void*);
	MediaCommandCreateAudioTrack();

private:

};

#endif
