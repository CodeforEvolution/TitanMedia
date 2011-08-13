/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_CLIP_INFO
#define __MEDIA_INFO_COMMAND_GET_CLIP_INFO

#include "EMCommand.h"

class MediaInfoCommandGetClipInfo : public EMCommand
{
public:
	MediaInfoCommandGetClipInfo();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaClipID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	EMMediaClipInfo m_oReturnValue;

};

#endif
