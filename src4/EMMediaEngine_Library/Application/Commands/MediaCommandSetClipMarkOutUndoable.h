/*******************************************************
* Creator: Jesper Svensson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SET_CLIP_MARK_OUT_UNDOABLE
#define __MEDIA_COMMAND_SET_CLIP_MARK_OUT_UNDOABLE

#include "EMCommand.h"

class EMMediaClip;
class EMMediaTrack;

class MediaCommandSetClipMarkOutUndoable : public EMCommand
{
public:
	MediaCommandSetClipMarkOutUndoable();
	MediaCommandSetClipMarkOutUndoable(int32 p_vClipID, int64 p_vOldMarkOut);

	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void*);
	bool RequiresParameters();
	bool IsUndoable();
	EMCommand* CloneAndClear();
	void UndoE();

private:
	int32 m_vClipID;
	int64 m_vOldMarkOut;
};

#endif
