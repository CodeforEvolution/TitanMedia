/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_VIDEO_ASPECT_RATIO
#define __MEDIA_INFO_COMMAND_GET_VIDEO_ASPECT_RATIO

#include "EMCommand.h"

class MediaInfoCommandGetVideoAspectRatio : public EMCommand
{
public:
	MediaInfoCommandGetVideoAspectRatio();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaPoolEntryID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();
};

#endif
