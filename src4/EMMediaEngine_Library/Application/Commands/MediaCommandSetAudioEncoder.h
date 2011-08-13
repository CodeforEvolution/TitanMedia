/*******************************************************
* Creator: Jesper Svensson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SET_AUDIO_ENCODER
#define __MEDIA_COMMAND_SET_AUDIO_ENCODER

#include "EMCommand.h"

class MediaCommandSetAudioEncoder : public EMCommand
{
public:
	MediaCommandSetAudioEncoder();
	void ExecuteE() {};
	void* ExecuteE(void* p_vEncoderID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:

};

#endif
