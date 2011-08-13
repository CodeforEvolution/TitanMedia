/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_CLIP_NUM_CHANNELS
#define __MEDIA_INFO_COMMAND_GET_CLIP_NUM_CHANNELS

#include "EMCommand.h"

class MediaInfoCommandGetClipNumChannels : public EMCommand
{
public:
	MediaInfoCommandGetClipNumChannels();
	void ExecuteE() {};
	void* ExecuteE(void* p_upFileDescID, void* p_upThisIsNotUsed, void* p_upThisIsNotUsedEither);
	bool RequiresParameters();
	bool IsUndoable();

private:
	int64 m_vReturnValue;

};

#endif
