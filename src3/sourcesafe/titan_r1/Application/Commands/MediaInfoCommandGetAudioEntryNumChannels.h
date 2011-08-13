/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_NUM_CHANNELS
#define __MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_NUM_CHANNELS

#include "EMCommand.h"

class MediaInfoCommandGetAudioEntryNumChannels : public EMCommand
{
public:
	MediaInfoCommandGetAudioEntryNumChannels();
	void ExecuteE() {};

	//Takes an int32* of the entry id number and returns an int64* describing the number of channels of that audio entry's file/data source.
	void* ExecuteE(void* p_upPoolEntryID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	int64 m_vReturnValue;

};

#endif
