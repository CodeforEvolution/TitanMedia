/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_VIDEO_ENTRY_NUM_FRAMES
#define __MEDIA_INFO_COMMAND_GET_VIDEO_ENTRY_NUM_FRAMES

#include "EMCommand.h"

class MediaInfoCommandGetVideoEntryNumFrames : public EMCommand
{
public:
	MediaInfoCommandGetVideoEntryNumFrames();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaPoolEntryID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	string m_vReturnValue;
	int64 m_vDummy;
};

#endif
