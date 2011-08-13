/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_EFFECT_ENTRY_INFO
#define __MEDIA_INFO_COMMAND_GET_EFFECT_ENTRY_INFO

#include "EMCommand.h"

class MediaInfoCommandGetEffectEntryInfo : public EMCommand
{
public:
	MediaInfoCommandGetEffectEntryInfo();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaEffectEntryID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	EMMediaEffectEntryInfo m_oReturnValue;

};

#endif
