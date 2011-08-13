/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_CLIP_FILE_NAME
#define __MEDIA_INFO_COMMAND_GET_CLIP_FILE_NAME

#include "EMCommand.h"

class MediaInfoCommandGetClipFileName : public EMCommand
{
public:
	MediaInfoCommandGetClipFileName();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaClipID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	string m_vReturnValue;
};

#endif
