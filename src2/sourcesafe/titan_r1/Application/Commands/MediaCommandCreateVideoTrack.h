/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/
#include "MediaCommandCreateTrack.h"

#ifndef __MEDIA_COMMAND_CREATE_VIDEO_TRACK
#define __MEDIA_COMMAND_CREATE_VIDEO_TRACK

class MediaCommandCreateVideoTrack : public MediaCommandCreateTrack
{
public:
	void ExecuteE();
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
	MediaCommandCreateVideoTrack();

private:

};

#endif
