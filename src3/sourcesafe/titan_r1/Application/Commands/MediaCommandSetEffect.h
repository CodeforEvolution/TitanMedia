/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SET_EFFECT
#define __MEDIA_COMMAND_SET_EFFECT

#include "EMCommand.h"

class MediaCommandSetEffect : public EMCommand
{
public:
	MediaCommandSetEffect();
	void ExecuteE() {};
	void* ExecuteE(void* p_upPluginEntryID, void* p_upEffectTrackID, void* p_upSlotNumber);
	bool RequiresParameters();
	void UndoE();
	bool IsUndoable();
	EMCommand* CloneAndClear();
	
private:
	int32 m_vReturnValue;
};

#endif
