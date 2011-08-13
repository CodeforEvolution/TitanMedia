/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_CREATE_EFFECT_TRACK
#define __MEDIA_COMMAND_CREATE_EFFECT_TRACK

#include "EMCommand.h"

class MediaCommandCreateEffectTrack : public EMCommand
{
public:
	MediaCommandCreateEffectTrack();
	void ExecuteE() {};
	void* ExecuteE(void* p_upName, void*, void*);
	bool RequiresParameters();
	void UndoE();
	bool IsUndoable();
	EMCommand* CloneAndClear();
	
private:
	int32 m_vReturnValue;
};

#endif
