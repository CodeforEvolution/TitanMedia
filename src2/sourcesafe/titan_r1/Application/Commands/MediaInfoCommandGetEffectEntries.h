/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_EFFECT_ENTRIES
#define __MEDIA_INFO_COMMAND_GET_EFFECT_ENTRIES

#include "EMCommand.h"

class MediaInfoCommandGetEffectEntries : public EMCommand
{
public:
	MediaInfoCommandGetEffectEntries();
	void ExecuteE() {};
	void* ExecuteE(void* p_upList, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	list<int32> m_oReturnValue;
};

#endif
