/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_BIT_DEPTH
#define __MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_BIT_DEPTH

#include "EMCommand.h"

class MediaInfoCommandGetAudioEntryBitDepth : public EMCommand
{
public:
	MediaInfoCommandGetAudioEntryBitDepth();
	void ExecuteE() {};

	//Takes a Media pool entry ID (int32*) and returns an int64* describing the number bits per sample for the entry's file/data source.
	void* ExecuteE(void* p_upPoolEntryID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	int64 m_vReturnValue;

};

#endif
