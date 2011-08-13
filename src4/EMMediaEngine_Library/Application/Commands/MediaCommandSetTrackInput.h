/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SET_TRACK_INPUT
#define __MEDIA_COMMAND_SET_TRACK_INPUT

#include "EMCommand.h"
class EMMediaTrack;
class EMBeRealtimeInputDescriptor;

class MediaCommandSetTrackInput : public EMCommand
{
public:
	MediaCommandSetTrackInput(int32 p_vRecentTrackID = -1, int32 p_vRecentInputID = -1);
	void ExecuteE() {};
	void* ExecuteE(void* p_upTrackID, void* p_upInputID, void*);
	bool RequiresParameters();
	void UndoE();
	bool IsUndoable();
	EMCommand* CloneAndClear();
	
private:
	int32 m_vRecentTrackID;
	int32 m_vRecentInputID;
};

#endif
