/*******************************************************
* Creator: Kaneda
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_CREATE_VIDEO_TRANSITION_CLIP
#define __MEDIA_COMMAND_CREATE_VIDEO_TRANSITION_CLIP

#include "EMCommand.h"

class MediaCommandCreateVideoTransitionClip : public EMCommand
{
public:
	MediaCommandCreateVideoTransitionClip();
	MediaCommandCreateVideoTransitionClip(int32 p_vID);
	~MediaCommandCreateVideoTransitionClip();

	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaClipID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();
	void UndoE();
	EMCommand* CloneAndClear();

private:
	int32 m_vID;

};

#endif