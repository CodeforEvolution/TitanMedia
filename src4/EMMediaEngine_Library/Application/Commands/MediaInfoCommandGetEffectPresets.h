/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_EFFECT_PRESETS
#define __MEDIA_INFO_COMMAND_GET_EFFECT_PRESETS

#include "EMCommand.h"

class MediaInfoCommandGetEffectPresets : public EMCommand
{
public:
	MediaInfoCommandGetEffectPresets();
	~MediaInfoCommandGetEffectPresets();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaEffectID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	void DeallocateArrays();
	list<char*> m_oReturnValue;
};

#endif
