/*******************************************************
* Creator: Jesper Svensson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SET_VIDEO_ENCODER
#define __MEDIA_COMMAND_SET_VIDEO_ENCODER

#include "EMCommand.h"

class MediaCommandSetVideoEncoder : public EMCommand
{
public:
	MediaCommandSetVideoEncoder();
	void ExecuteE() {};
	void* ExecuteE(void* p_vEncoderID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:

};

#endif
