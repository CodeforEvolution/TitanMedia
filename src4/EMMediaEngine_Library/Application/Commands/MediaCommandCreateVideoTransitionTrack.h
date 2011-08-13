/*******************************************************
* Creator: Kaneda
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/
#include "EMCommand.h"

class EMMediaTrack;

#ifndef __MEDIA_COMMAND_CREATE_VIDEO_TRANSITION_TRACK
#define __MEDIA_COMMAND_CREATE_VIDEO_TRANSITION_TRACK

class MediaCommandCreateVideoTransitionTrack : public EMCommand
{
public:
	MediaCommandCreateVideoTransitionTrack();
	MediaCommandCreateVideoTransitionTrack(int32 p_vID);
	~MediaCommandCreateVideoTransitionTrack();


	void ExecuteE();
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);

	bool RequiresParameters();
	bool IsUndoable();
	void UndoE();

	EMCommand* CloneAndClear();
protected:
	EMMediaTrack* FindParentTrackE(uint32 p_vParentID);
private:
	uint32 m_vID;
};

#endif
