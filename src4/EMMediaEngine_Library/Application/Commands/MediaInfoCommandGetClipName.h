/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_CLIP_NAME
#define __MEDIA_INFO_COMMAND_GET_CLIP_NAME

#include "EMCommand.h"

class MediaInfoCommandGetClipName : public EMCommand
{
public:
	MediaInfoCommandGetClipName();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaClipID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	char m_vpReturnValue[1024];
};

#endif
