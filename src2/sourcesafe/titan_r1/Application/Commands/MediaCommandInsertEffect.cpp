#include "MediaCommandInsertEffect.h"

#include "EMMediaEffectTrack.h"
#include "EMMediaEffectTrackRepository.h"
#include "EMPluginEntry.h"
#include "EMPluginRepository.h"
#include "EMPlugin.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"

MediaCommandInsertEffect::MediaCommandInsertEffect()
	:	m_vReturnValue(-1)
{
}

//Takes two IDs - the plugin entry id and the effect track id, and returns the id of the new instantiated plugin
void* MediaCommandInsertEffect::ExecuteE(void* p_upPluginEntryID, void* p_upEffectTrackID, void* p_upSlotNumber)
{ 
	try
	{
		int32 vEntryID = *static_cast<int32*>(p_upPluginEntryID);
		int32 vFXTrackID = *static_cast<int32*>(p_upEffectTrackID);
		int32 vSlotNumber = *static_cast<uint32*>(p_upSlotNumber);

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
			opFXTrack -> InsertEffect(opNewPlugin, vSlotNumber);
			m_vReturnValue = opNewPlugin -> GetID();
			EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
			return static_cast<void*>(&m_vReturnValue);
		}
		else
			EMDebugger("ERROR! Could not instantiate EMPluginEntry into an EMPlugin!");
	}
	catch(...)
	{
		eo << "Exception in InsertEffect!" << ef;
	}
	return NULL;
}	

bool MediaCommandInsertEffect::RequiresParameters()
{
	return true;
}

void MediaCommandInsertEffect::UndoE()
{
}

bool MediaCommandInsertEffect::IsUndoable()
{
	return false;
}

EMCommand* MediaCommandInsertEffect::CloneAndClear()
{
	return EM_new MediaCommandInsertEffect();
}

