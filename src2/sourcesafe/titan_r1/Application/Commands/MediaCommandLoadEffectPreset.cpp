#include "MediaCommandLoadEffectPreset.h"

#include "EMMediaEffectTrackRepository.h"
#include "EMPlugin.h"
#include "EMMediaEffectTrack.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"

MediaCommandLoadEffectPreset::MediaCommandLoadEffectPreset()
{
}

void* MediaCommandLoadEffectPreset::ExecuteE(void* p_upMediaEffectID, void* p_upPresetName, void*)
{
	int32 vMediaEffectID = *static_cast<int32*>(p_upMediaEffectID);
	char* vpPresetName = static_cast<char*>(p_upPresetName);

	EMMediaEffectTrackRepository* opFXTrackRep = EMMediaEffectTrackRepository::Instance();

	bool vBreakLoop = true;
	EMPlugin* opWantedPlugin = NULL;

	opFXTrackRep -> Rewind();
	while(opFXTrackRep -> Current() != NULL && ! vBreakLoop)
	{
		EMMediaEffectTrack* opFXTrack = opFXTrackRep -> Current();
		opWantedPlugin = opFXTrack -> FindEffect(vMediaEffectID);
		if(opWantedPlugin != NULL)
			break;
		opFXTrackRep -> Next();
	}

	if(opWantedPlugin != NULL)
	{
		list<EMPluginPreset*>* opPresets = opWantedPlugin -> GetPresets();
	
		list<EMPluginPreset*>::const_iterator oIter;
		for(oIter = opPresets -> begin(); oIter != opPresets -> end(); oIter++)
		{
			EMPluginPreset* opPreset = (*oIter);
			if(opPreset -> m_oName == string(const_cast<const char*>(vpPresetName)))
			{
				m_vReturnValue = opWantedPlugin -> ActivatePreset(opPreset -> m_vID);
			}
		}
	}
	else
		EMDebugger("ERROR! Could not find the effect with the specified ID!");

	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
	return static_cast<void*>(&m_vReturnValue);
}

bool MediaCommandLoadEffectPreset::RequiresParameters()
{
	return true;
}

bool MediaCommandLoadEffectPreset::IsUndoable()
{
	return false;
}