/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_LOAD_EFFECT_PRESET
#define __MEDIA_COMMAND_LOAD_EFFECT_PRESET

#include "EMCommand.h"

class MediaCommandLoadEffectPreset : public EMCommand
{
public:
	MediaCommandLoadEffectPreset();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaEffectID, void* p_upPresetName, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	bool m_vReturnValue;
};

#endif
