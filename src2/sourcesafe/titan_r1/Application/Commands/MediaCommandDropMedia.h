/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_DROP_MEDIA
#define __MEDIA_COMMAND_DROP_MEDIA

#include "EMCommand.h"

class MediaCommandDropMedia : public EMCommand
{
public:
	MediaCommandDropMedia();
	MediaCommandDropMedia(int32 p_vNewClipID);
	void* ExecuteE(void* p_upEntryID, void* p_upTrackID, void* p_upAtTime);
	void ExecuteE() {};
	bool RequiresParameters();
	bool IsUndoable();
	void UndoE();
	EMCommand* CloneAndClear();

private:
	int32 m_vReturnValue;

	int32 m_vNewClipID;
};

#endif
