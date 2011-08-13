#include "MediaCommandSetEffect.h"

#include "EMMediaEffectTrack.h"
#include "EMMediaEffectTrackRepository.h"
#include "EMPluginEntry.h"
#include "EMPluginRepository.h"
#include "EMPlugin.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"

MediaCommandSetEffect::MediaCommandSetEffect()
	:	m_vReturnValue(-1)
{
}

//Takes two IDs - the plugin entry id and the effect track id, and returns the id of the new instantiated plugin
void* MediaCommandSetEffect::ExecuteE(void* p_upPluginEntryID, void* p_upEffectTrackID, void* p_upSlotNumber)
{
	try
	{
		int32 vEntryID = *static_cast<int32*>(p_upPluginEntryID); //New entry ID
		int32 vFXTrackID = *static_cast<int32*>(p_upEffectTrackID); //Existing track ID
		int32 vSlotNumber = *static_cast<uint32*>(p_upSlotNumber); //Already occupied slot number

		EMPluginEntry* opPluginEntry = EMPluginRepository::Instance() -> Find(vEntryID);
		EMMediaEffectTrack* opFXTrack = EMMediaEffectTrackRepository::Instance() -> Find(vFXTrackID);
 
		if(opPluginEntry == NULL)
			EMDebugger("ERROR! Could not locate the specified plugin entry ID!");
		if(opFXTrack == NULL)
			EMDebugger("ERROR! Could not locate the specified effect track ID!");

		EMPlugin* opNewPlugin = opPluginEntry -> InstantiateNativePlugin();
		if(opNewPlugin != NULL)
		{
			opNewPlugin -> ShowDialog();
			opFXTrack -> SetEffect(opNewPlugin, vSlotNumber);
			EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
			m_vReturnValue = opNewPlugin -> GetID();
			return static_cast<void*>(&m_vReturnValue);
		}
		else
			EMDebugger("ERROR! Could not instantiate EMPluginEntry into an EMPlugin!");
	}
	catch(...)
	{
		eo << "Exception in SetEffect!" << ef;
	}
	return NULL;
}	

bool MediaCommandSetEffect::RequiresParameters()
{
	return true;
}

void MediaCommandSetEffect::UndoE()
{
}

bool MediaCommandSetEffect::IsUndoable()
{
	return false;
}

EMCommand* MediaCommandSetEffect::CloneAndClear()
{
	return EM_new MediaCommandSetEffect();
}

