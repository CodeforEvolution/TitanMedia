/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_AUDIO_RENDER_OUTPUT
#define __MEDIA_INFO_COMMAND_GET_AUDIO_RENDER_OUTPUT

#include "EMCommand.h"

class MediaInfoCommandGetAudioRenderOutput : public EMCommand
{
public:
	MediaInfoCommandGetAudioRenderOutput();
	void ExecuteE() {};
	void* ExecuteE(void*, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	int32 m_vReturnValue;
};

#endif
