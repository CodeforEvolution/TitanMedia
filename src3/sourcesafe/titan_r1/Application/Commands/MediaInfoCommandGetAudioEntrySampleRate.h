/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_SAMPLE_RATE
#define __MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_SAMPLE_RATE

#include "EMCommand.h"

class MediaInfoCommandGetAudioEntrySampleRate : public EMCommand
{
public:
	MediaInfoCommandGetAudioEntrySampleRate();
	void ExecuteE() {};

	//Takes a Media pool entry ID (int32*) and returns an int64* describing the number of audio samples per second in the entry's file/data source.
	void* ExecuteE(void* p_upPoolEntryID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	int64 m_vReturnValue;

};

#endif
