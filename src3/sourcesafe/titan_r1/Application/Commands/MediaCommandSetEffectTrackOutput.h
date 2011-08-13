/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SET_EFFECT_TRACK_OUTPUT
#define __MEDIA_COMMAND_SET_EFFECT_TRACK_OUTPUT

#include "EMCommand.h"
class EMMediaTrack;
class EMBeRealtimeOutputDescriptor;

class MediaCommandSetEffectTrackOutput : public EMCommand
{
public:
	MediaCommandSetEffectTrackOutput(int32 p_vRecentTrackID = -1, int32 p_vRecentOutputID = -1);
	void ExecuteE() {};
	void* ExecuteE(void* p_upFXTrackID, void* p_upOutputID, void* p_upOutputIsEffectTrack);
	bool RequiresParameters();
	void UndoE();
	bool IsUndoable();
	EMCommand* CloneAndClear();
	
private:
	int32 m_vRecentTrackID;
	int32 m_vRecentOutputID;
};

#endif
