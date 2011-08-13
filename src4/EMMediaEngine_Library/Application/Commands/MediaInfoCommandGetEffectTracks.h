/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_EFFECT_TRACKS
#define __MEDIA_INFO_COMMAND_GET_EFFECT_TRACKS

#include "EMCommand.h"

class MediaInfoCommandGetEffectTracks : public EMCommand
{
public:
	MediaInfoCommandGetEffectTracks();
	void ExecuteE() {};
	void* ExecuteE(void*, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	list<int32> m_oReturnValue;
};

#endif
