/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_VIDEO_ENTRY_FRAME_RATE
#define __MEDIA_INFO_COMMAND_GET_VIDEO_ENTRY_FRAME_RATE

#include "EMCommand.h"

class MediaInfoCommandGetVideoEntryFrameRate : public EMCommand
{
public:
	MediaInfoCommandGetVideoEntryFrameRate();
	void ExecuteE() {};

	//Takes a Media pool entry ID (int32*) and returns an int64* describing the number of video frames per second in the entry's file/data source.
	void* ExecuteE(void* p_upPoolEntryID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	int64 m_vReturnValue;

};

#endif
