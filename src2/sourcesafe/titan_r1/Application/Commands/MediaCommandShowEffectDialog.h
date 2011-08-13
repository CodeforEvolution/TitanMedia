/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SHOW_EFFECT_DIALOG
#define __MEDIA_COMMAND_SHOW_EFFECT_DIALOG

#include "EMCommand.h"

class MediaCommandShowEffectDialog : public EMCommand
{
public:
	MediaCommandShowEffectDialog();
	void ExecuteE() {};
	void* ExecuteE(void* p_upPluginID, void* p_upEffectTrackID, void*);
	bool RequiresParameters();
//	void UndoE();
	bool IsUndoable();
//	EMCommand* CloneAndClear();
	
private:
//	int32 m_vPluginID;
//	int32 m_vTrackID;
//	uint32 m_vOldDryWetMix;
};

#endif
