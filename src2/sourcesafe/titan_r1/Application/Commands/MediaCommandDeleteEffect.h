/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_DELETE_EFFECT
#define __MEDIA_COMMAND_DELETE_EFFECT

#include "EMCommand.h"

class MediaCommandDeleteEffect : public EMCommand
{
public:
	MediaCommandDeleteEffect();
	void ExecuteE() {};
	void* ExecuteE(void* p_upEffectTrackID, void* p_upSlotNumber, void*);
	bool RequiresParameters();
	bool IsUndoable();
};

#endif
