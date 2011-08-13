/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_AUDIO_TRACK_IDS
#define __MEDIA_INFO_COMMAND_GET_AUDIO_TRACK_IDS

#include "EMCommand.h"

class MediaInfoCommandGetAudioTrackIDs : public EMCommand
{
public:
	MediaInfoCommandGetAudioTrackIDs();
	void ExecuteE() {};
	void* ExecuteE(void*, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();
	
private:
	list<int32> m_oReturnValue;

};

#endif
