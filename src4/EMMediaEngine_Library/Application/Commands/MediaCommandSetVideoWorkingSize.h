/*******************************************************
* Creator: Jesper Svensson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SET_VIDEO_WORKING_SIZE
#define __MEDIA_COMMAND_SET_VIDEO_WORKING_SIZE

#include "EMCommand.h"

class MediaCommandSetVideoWorkingSize : public EMCommand
{
public:
	MediaCommandSetVideoWorkingSize();
	void ExecuteE() {};
	void* ExecuteE(void* p_vEncoderID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:

};

#endif
