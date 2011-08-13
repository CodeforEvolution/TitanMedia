#include "MediaInfoCommandGetEffectInfo.h"

#include "EMMediaEffectTrackRepository.h"
#include "EMPlugin.h"
#include "EMMediaEffectTrack.h"

MediaInfoCommandGetEffectInfo::MediaInfoCommandGetEffectInfo()
{
}

void* MediaInfoCommandGetEffectInfo::ExecuteE(void* p_upMediaEffectID, void*, void*)
{
	int32 vMediaEffectID = *static_cast<int32*>(p_upMediaEffectID);

	EMMediaEffectTrackRepository* opFXTrackRep = EMMediaEffectTrackRepository::Instance();

	bool vBreakLoop = false;
	EMPlugin* opWantedPlugin = NULL;
	EMMediaEffectTrack* opFXTrack = NULL;

	opFXTrackRep -> LockContainer();
	opFXTrackRep -> Rewind();
	while(opFXTrackRep -> Current() != NULL && ! vBreakLoop)
	{
		if(!(opFXTrackRep -> Current() -> IsObjectDeleted()))
		{
			opFXTrack = opFXTrackRep -> Current();
			opWantedPlugin = opFXTrack -> FindEffect(vMediaEffectID);
			if(opWantedPlugin != NULL)
				break;
		}
		opFXTrackRep -> Next();
	}
	opFXTrackRep -> UnlockContainer();

	if(opWantedPlugin != NULL && opFXTrack != NULL)
	{
		m_oReturnValue.m_vID = opWantedPlugin -> GetID();
		m_oReturnValue.m_eType = (EMMediaType) opWantedPlugin -> GetType();
		m_oReturnValue.m_ePluginType = opWantedPlugin -> GetPluginType();
		m_oReturnValue.m_oPluginName = opWantedPlugin -> GetName();
		m_oReturnValue.m_vSlotNumber = opFXTrack -> GetSlotFor(opWantedPlugin);
		m_oReturnValue.m_vEntryID = opWantedPlugin -> GetEntryID();
		m_oReturnValue.m_vDryWetMix = opWantedPlugin -> GetDryWetMix();
	}
	else
		EMDebugger("ERROR! Could not find the effect with the specified ID!");

	return static_cast<void*>(&m_oReturnValue);
}

bool MediaInfoCommandGetEffectInfo::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetEffectInfo::IsUndoable()
{
	return false;
}