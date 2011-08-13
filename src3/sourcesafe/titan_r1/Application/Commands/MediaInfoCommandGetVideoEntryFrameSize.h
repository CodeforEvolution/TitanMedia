/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_VIDEO_ENTRY_FRAME_SIZE
#define __MEDIA_INFO_COMMAND_GET_VIDEO_ENTRY_FRAME_SIZE

#include "EMCommand.h"

class MediaInfoCommandGetVideoEntryFrameSize : public EMCommand
{
public:
	MediaInfoCommandGetVideoEntryFrameSize();
	void ExecuteE() {};

	//Takes a Media pool entry ID (int32*) and two unsigned int64 (uint64) pointers that will be set to X and Y sizes respectively, of the video frames of the entry's file/data source.
	void* ExecuteE(void* p_upPoolEntryID, void* p_upXSizePointer, void* p_upYSizePointer);
	bool RequiresParameters();
	bool IsUndoable();
};

#endif
