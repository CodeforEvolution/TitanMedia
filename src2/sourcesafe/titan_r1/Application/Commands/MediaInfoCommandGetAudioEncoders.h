/*******************************************************
* Creator: Jesper Svensson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_AUDIO_ENCODERS
#define __MEDIA_INFO_COMMAND_GET_AUDIO_ENCODERS

#include "EMCommand.h"

class MediaInfoCommandGetAudioEncoders : public EMCommand
{
public:
	MediaInfoCommandGetAudioEncoders();
	void ExecuteE() {};
	void* ExecuteE(void* p_upIDs, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:

};

#endif
