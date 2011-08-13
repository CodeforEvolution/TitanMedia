/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_CLIP_MARK_OUT
#define __MEDIA_INFO_COMMAND_GET_CLIP_MARK_OUT

#include "EMCommand.h"

class MediaInfoCommandGetClipMarkOut : public EMCommand
{
public:
	MediaInfoCommandGetClipMarkOut();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaClipID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	int64 m_vReturnValue;

};

#endif
