/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SET_EFFECT_DRY_WET_MIX
#define __MEDIA_COMMAND_SET_EFFECT_DRY_WET_MIX

#include "EMCommand.h"

class MediaCommandSetEffectDryWetMix : public EMCommand
{
public:
	MediaCommandSetEffectDryWetMix(int32 p_vPlugin = -1, int32 p_vTrack = -1, uint32 p_vMix = 0);
	void ExecuteE() {};
	void* ExecuteE(void* p_upPluginID, void* p_upEffectTrackID, void* p_upDryWetRatio);
	bool RequiresParameters();
	void UndoE();
	bool IsUndoable();
	EMCommand* CloneAndClear();
	
private:
	int32 m_vPluginID;
	int32 m_vTrackID;
	uint32 m_vOldDryWetMix;
};

#endif
