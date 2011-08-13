/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_CURRENT_VIDEO_PROCESSING_POSITION
#define __MEDIA_INFO_COMMAND_GET_CURRENT_VIDEO_PROCESSING_POSITION

#include "EMCommand.h"

class MediaInfoCommandGetCurrentVideoProcessingPosition : public EMCommand
{
public:
	MediaInfoCommandGetCurrentVideoProcessingPosition();
	void ExecuteE() {};
	void* ExecuteE(void*, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	int64 m_vReturnValue;
};

#endif
