/*******************************************************
* Creator: Jesper Svensson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SET_CLIP_MARK_OUT
#define __MEDIA_COMMAND_SET_CLIP_MARK_OUT

#include "EMCommand.h"

class EMMediaClip;
class EMMediaTrack;

class MediaCommandSetClipMarkOut : public EMCommand
{
public:
	MediaCommandSetClipMarkOut();
//	void ExecuteE() {};

	//parameter 1 = clip ID <int32>, parameter 2 = new song time in frames <int64>, parameter 3 = new track id (if changed) <int32>
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
};

#endif
