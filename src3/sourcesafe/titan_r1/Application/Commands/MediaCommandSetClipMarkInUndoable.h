/*******************************************************
* Creator: Jesper Svensson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SET_CLIP_MARK_IN_UNDOABLE
#define __MEDIA_COMMAND_SET_CLIP_MARK_IN_UNDOABLE

#include "EMCommand.h"

class EMMediaClip;
class EMMediaTrack;

class MediaCommandSetClipMarkInUndoable : public EMCommand
{
public:
	MediaCommandSetClipMarkInUndoable();
	MediaCommandSetClipMarkInUndoable(int32 p_vClipID, int64 p_vMarkIn);
	void ExecuteE() {};

	//parameter 1 = clip ID <int32>, parameter 2 = new song time in frames <int64>, parameter 3 = new track id (if changed) <int32>
	void* ExecuteE(void* p_upClipID, void* p_upNewOffset, void* p_upIsUndoableMove);
	bool RequiresParameters();
	void UndoE();
	bool IsUndoable();
	EMCommand* CloneAndClear();

private:
	int32 m_vClipID;
	int64 m_vOldMarkIn;
};

#endif
