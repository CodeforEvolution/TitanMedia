/*******************************************************
* Creator: Jesper Svensson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_NUMBER_AUDIO_ENCODERS
#define __MEDIA_INFO_COMMAND_GET_NUMBER_AUDIO_ENCODERS

#include "EMCommand.h"

class MediaInfoCommandGetNumberAudioEncoders : public EMCommand
{
public:
	MediaInfoCommandGetNumberAudioEncoders();
	void ExecuteE() {};
	void* ExecuteE(void*, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
 int32 m_vNumAudioEnc;
};

#endif
