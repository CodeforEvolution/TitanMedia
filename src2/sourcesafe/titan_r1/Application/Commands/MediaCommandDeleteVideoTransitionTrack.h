/*******************************************************
* Creator: Kaneda
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_DELETE_VIDEO_TRANSITION_TRACK
#define __MEDIA_COMMAND_DELETE_VIDEO_TRANSITION_TRACK

#include "EMCommand.h"
#include "MediaCommandDeleteTrack.h"

class EMMediaTrack;

class MediaCommandDeleteVideoTransitionTrack : public MediaCommandDeleteTrack
{
public:
	MediaCommandDeleteVideoTransitionTrack();

	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
private:
	EMMediaTrack* GetTrack(uint32 p_vID);
};

#endif
