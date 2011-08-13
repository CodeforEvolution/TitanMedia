/*******************************************************
* Creator: Kaneda
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_RESIZE_CLIP
#define __MEDIA_COMMAND_RESIZE_CLIP

#include "EMCommand.h"

class EMMediaClip;
class EMMediaTrack;
class EMVideoTransitionClip;

class MediaCommandResizeClip : public EMCommand
{
public:
	MediaCommandResizeClip(EMVideoTransitionClip* p_opClip = NULL, int64 p_vOldStart = 0, int64 p_vOldStop = 0);
	void ExecuteE() {};

	//parameter 1 = clip ID <int32>, parameter 2 = new song time in frames <int64>, parameter 3 = new song time stop.
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
	bool RequiresParameters();
	void UndoE();
	bool IsUndoable();
	EMCommand* CloneAndClear();

private:
	EMVideoTransitionClip* m_opClip;
	int64 m_vOldStart;
	int64 m_vOldStop;
};

#endif
