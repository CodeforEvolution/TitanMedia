/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_EFFECT_TRACK_ID
#define __MEDIA_INFO_COMMAND_GET_EFFECT_TRACK_ID

#include "EMCommand.h"

class MediaInfoCommandGetEffectTrackID : public EMCommand
{
public:
	MediaInfoCommandGetEffectTrackID();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaTrackID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	int32 m_vReturnValue;

};

#endif
