/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_CLIP_MARK_IN
#define __MEDIA_INFO_COMMAND_GET_CLIP_MARK_IN

#include "EMCommand.h"

class MediaInfoCommandGetClipMarkIn : public EMCommand
{
public:
	MediaInfoCommandGetClipMarkIn();
	~MediaInfoCommandGetClipMarkIn();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaClipID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	int64 m_vReturnValue;

};

#endif
