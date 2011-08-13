/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"
class EMMediaPoolEntry;


#ifndef __MEDIA_INFO_COMMAND_GET_VIDEO_CLIP_CONTENT
#define __MEDIA_INFO_COMMAND_GET_VIDEO_CLIP_CONTENT

#include "EMCommand.h"

class MediaInfoCommandGetVideoClipContent : public EMCommand
{
public:
	MediaInfoCommandGetVideoClipContent();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaPoolEntryID, void* p_upTime, void* p_upHeight);
	bool RequiresParameters();
	bool IsUndoable();

private:
EMMediaPoolEntry* m_opEntry;
int32 m_vMediaPoolEntryID;
};

#endif
