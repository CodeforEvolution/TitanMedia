/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_VIDEO_RENDER_OUTPUT
#define __MEDIA_INFO_COMMAND_GET_VIDEO_RENDER_OUTPUT

#include "EMCommand.h"

class MediaInfoCommandGetVideoRenderOutput : public EMCommand
{
public:
	MediaInfoCommandGetVideoRenderOutput();
	void ExecuteE() {};
	void* ExecuteE(void*, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	int32 m_vReturnValue;
};

#endif
