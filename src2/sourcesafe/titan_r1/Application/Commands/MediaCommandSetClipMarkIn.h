/*******************************************************
* Creator: Jesper Svensson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SET_CLIP_MARK_IN
#define __MEDIA_COMMAND_SET_CLIP_MARK_IN

#include "EMCommand.h"

class EMMediaClip;
class EMMediaTrack;

class MediaCommandSetClipMarkIn : public EMCommand
{
public:
	MediaCommandSetClipMarkIn();
	void ExecuteE() {};

	void* ExecuteE(void* p_upClipID, void* p_upNewOffset, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
};

#endif
