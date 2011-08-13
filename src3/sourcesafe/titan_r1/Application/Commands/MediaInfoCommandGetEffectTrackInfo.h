/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_EFFECT_TRACK_INFO
#define __MEDIA_INFO_COMMAND_GET_EFFECT_TRACK_INFO

#include "EMCommand.h"

class MediaInfoCommandGetEffectTrackInfo : public EMCommand
{
public:
	MediaInfoCommandGetEffectTrackInfo();
	void ExecuteE() {};
	void* ExecuteE(void* p_upEffectTrackID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	EMMediaEffectTrackInfo m_oReturnValue;

};

#endif
