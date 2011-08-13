#include "MediaCommandCreateAudioTrack.h"

#include <windows.h>

MediaCommandCreateAudioTrack::MediaCommandCreateAudioTrack()
{
//	MessageBox(NULL, "MediaCommandCreateAudioTrack created!", "", MB_OK);
}

void MediaCommandCreateAudioTrack::ExecuteE()
{
	ExecuteCreateTrack(EM_TYPE_ANY_AUDIO, "Untitled audio track");
}

void* MediaCommandCreateAudioTrack::ExecuteE(void* p_upTheNameOfTheTrack, void*, void*)
{
	const char* vpName = static_cast<const char*>(p_upTheNameOfTheTrack);
	uint32 vPrio = 0;
	return ExecuteCreateTrackP(EM_TYPE_ANY_AUDIO, vpName, vPrio);
}
