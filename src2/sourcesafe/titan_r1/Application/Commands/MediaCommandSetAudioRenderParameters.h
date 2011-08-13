/*******************************************************
* Creator: Martin Johansson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SET_AUDIO_RENDER_PARAMETERS
#define __MEDIA_COMMAND_SET_AUDIO_RENDER_PARAMETERS

#include "EMCommand.h"

class MediaCommandSetAudioRenderParameters : public EMCommand
{
public:
	MediaCommandSetAudioRenderParameters();
	void ExecuteE() {};
	void* ExecuteE(void* p_vEncoderID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

	bool m_vRet;

private:

};

#endif
