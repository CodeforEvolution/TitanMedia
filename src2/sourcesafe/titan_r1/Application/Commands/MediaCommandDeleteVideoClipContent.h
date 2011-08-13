/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"


#ifndef __MEDIA_COMMAND_DELETE_VIDEO_CLIP_CONTENT
#define __MEDIA_COMMAND_DELETE_VIDEO_CLIP_CONTENT

#include "EMCommand.h"

class MediaCommandDeleteVideoClipContent : public EMCommand
{
public:
	MediaCommandDeleteVideoClipContent();
	void ExecuteE() {};
	void* ExecuteE(void* p_opBitmap, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
};

#endif
