/*******************************************************
* Creator: Jesper Svensson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SET_CLIP_NAME
#define __MEDIA_COMMAND_SET_CLIP_NAME

#include "EMCommand.h"

class EMMediaClip;
class EMMediaTrack;

class MediaCommandSetClipName : public EMCommand
{
public:
	MediaCommandSetClipName(int32 p_vClipID = -1, const char* p_vpOldName = "");
	~MediaCommandSetClipName();
	void ExecuteE();
	void* ExecuteE(void* p_upMediaClipID, void* p_upString, void*);
	bool RequiresParameters();
	void UndoE();
	bool IsUndoable();
	EMCommand* CloneAndClear();

private:
	int32 m_vClipID;
//	string m_oOldName;
};

#endif
