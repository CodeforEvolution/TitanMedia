/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_VIDEO_ENTRY_FORMAT_STRING
#define __MEDIA_INFO_COMMAND_GET_VIDEO_ENTRY_FORMAT_STRING

#include "EMCommand.h"

class MediaInfoCommandGetVideoEntryFormatString : public EMCommand
{
public:
	MediaInfoCommandGetVideoEntryFormatString();
	void ExecuteE() { ExecuteE(NULL,NULL,NULL); };
	void* ExecuteE(void* p_upMediaEntryID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	char m_vpReturnValue[1024];
};

#endif
