/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_CLIP_POSITION
#define __MEDIA_INFO_COMMAND_GET_CLIP_POSITION

#include "EMCommand.h"

class MediaInfoCommandGetClipPosition : public EMCommand
{
public:
	MediaInfoCommandGetClipPosition();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaClipID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	int64 m_vReturnValue;
};

#endif
