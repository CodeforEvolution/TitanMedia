#include "MediaInfoCommandGetEffectPresets.h"

#include "EMMediaEffectTrackRepository.h"
#include "EMPlugin.h"
#include "EMMediaEffectTrack.h"

MediaInfoCommandGetEffectPresets::MediaInfoCommandGetEffectPresets()
{
}

MediaInfoCommandGetEffectPresets::~MediaInfoCommandGetEffectPresets()
{
	DeallocateArrays();
}

void* MediaInfoCommandGetEffectPresets::ExecuteE(void* p_upMediaEffectID, void*, void*)
{
	int32 vMediaEffectID = *static_cast<int32*>(p_upMediaEffectID);

	EMMediaEffectTrackRepository* opFXTrackRep = EMMediaEffectTrackRepository::Instance();

	bool vBreakLoop = true;
	EMPlugin* opWantedPlugin = NULL;

	opFXTrackRep -> Rewind();
	while(opFXTrackRep -> Current() != NULL && ! vBreakLoop)
	{
		if(!opFXTrackRep -> Current() ->IsObjectDeleted())
		{
			EMMediaEffectTrack* opFXTrack = opFXTrackRep -> Current();
			opWantedPlugin = opFXTrack -> FindEffect(vMediaEffectID);
			if(opWantedPlugin != NULL)
				break;
		}
		opFXTrackRep -> Next();
	}

	if(opWantedPlugin != NULL)
	{
		DeallocateArrays();

		list<EMPluginPreset*>* opPresets = opWantedPlugin -> GetPresets();

		list<EMPluginPreset*>::const_iterator oIter;
		for(oIter = opPresets -> begin(); oIter != opPresets -> end(); oIter++)
		{
			EMPluginPreset* opPreset = (*oIter);
			string oPresetName = opPreset -> m_oName;
			char* vpCharArray = EM_new char[oPresetName.size() + 1];
			memcpy(vpCharArray, oPresetName.c_str(), oPresetName.size());
			vpCharArray[oPresetName.size()] = '\0';
			m_oReturnValue.push_back(vpCharArray);
		}
	}
	else
		EMDebugger("ERROR! Could not find the effect with the specified ID!");

	return static_cast<void*>(&m_oReturnValue);
}

void MediaInfoCommandGetEffectPresets::DeallocateArrays()
{
	while(m_oReturnValue.size() > 0)
	{
		char* vpArray = m_oReturnValue.front();
		m_oReturnValue.pop_front();
		delete vpArray;
	}
}

bool MediaInfoCommandGetEffectPresets::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetEffectPresets::IsUndoable()
{
	return false;
}