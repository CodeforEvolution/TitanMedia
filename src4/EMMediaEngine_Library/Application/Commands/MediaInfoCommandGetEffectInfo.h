/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_EFFECT_INFO
#define __MEDIA_INFO_COMMAND_GET_EFFECT_INFO

#include "EMCommand.h"

class MediaInfoCommandGetEffectInfo : public EMCommand
{
public:
	MediaInfoCommandGetEffectInfo();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaEffectID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	EMMediaEffectInfo m_oReturnValue;

};

#endif
