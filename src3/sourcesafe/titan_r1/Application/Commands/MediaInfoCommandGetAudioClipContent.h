/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_AUDIO_CLIP_CONTENT
#define __MEDIA_INFO_COMMAND_GET_AUDIO_CLIP_CONTENT

#include "EMCommand.h"

class MediaInfoCommandGetAudioClipContent : public EMCommand
{
public:
	MediaInfoCommandGetAudioClipContent();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaPoolEntryID, void* p_upArray, void* p_upMoreParameters);
	bool RequiresParameters();
	bool IsUndoable();

private:
	int64 m_vReturnValue;
};

#endif
