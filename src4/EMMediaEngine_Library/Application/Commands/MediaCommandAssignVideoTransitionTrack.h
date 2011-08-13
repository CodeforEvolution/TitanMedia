/*******************************************************
* Creator: Kaneda
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_ASSIGN_VIDEO_TRANSITION_TRACK_TO_VIDEO_TRACK
#define __MEDIA_COMMAND_ASSIGN_VIDEO_TRANSITION_TRACK_TO_VIDEO_TRACK

#include "EMCommand.h"

class EMMediaTrack;

class MediaCommandAssignTransitionTrackToVideoTrack : public EMCommand
{
public:
	MediaCommandAssignTransitionTrackToVideoTrack();
	void ExecuteE() {};
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
	bool RequiresParameters();
	bool IsUndoable();

private:
	EMMediaTrack* GetTrack(uint32 p_vID);
};

#endif
