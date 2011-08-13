/*******************************************************
* Creator: Jesper Svensson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_VIDEO_DIMENSION
#define __MEDIA_INFO_COMMAND_GET_VIDEO_DIMENSION

#include "EMCommand.h"

class MediaInfoCommandGetVideoDimension : public EMCommand
{
public:

	MediaInfoCommandGetVideoDimension();

	void* ExecuteE(void* p_upMediaPoolEntryID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

};

#endif
