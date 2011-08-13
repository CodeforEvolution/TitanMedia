#include "MediaInfoCommandGetEffectTrackInfo.h"

#include "EMMediaEffectTrackRepository.h"
#include "EMMediaEffectTrack.h"
#include "EMMediaTrack.h"
#include "EMRealtimeOutputDescriptor.h"
#include "EMPlugin.h"

#include <list>

MediaInfoCommandGetEffectTrackInfo::MediaInfoCommandGetEffectTrackInfo()
{
}

void* MediaInfoCommandGetEffectTrackInfo::ExecuteE(void* p_upEffectTrackID, void*, void*)
{
	int32 vMediaEffectTrackID = *static_cast<int32*>(p_upEffectTrackID);
	
	EMMediaEffectTrack* opFXTrack = NULL;
	EMMediaEffectTrackRepository::Instance() -> LockContainer();
	try
	{
		opFXTrack = EMMediaEffectTrackRepository::Instance() -> Find(vMediaEffectTrackID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetEffectTrackInfo::ExecuteE");
	}
	EMMediaEffectTrackRepository::Instance() -> UnlockContainer();

	if(opFXTrack != NULL)
	{
		m_oReturnValue.m_vID = opFXTrack -> GetID();
		m_oReturnValue.m_eType = (EMMediaType) opFXTrack -> GetType(); 
		m_oReturnValue.m_oName = opFXTrack -> GetName(); 

		if(opFXTrack ->GetOutput() != NULL)
		{
			if(opFXTrack -> GetOutput() -> GetPosition() == EM_EFFECT)
				m_oReturnValue.m_vOutputIsEffectTrack = true;
			else
				m_oReturnValue.m_vOutputIsEffectTrack = false;

			m_oReturnValue.m_vOutputID = opFXTrack -> GetOutput() -> GetID();
		}
		else
		{
			m_oReturnValue.m_vOutputIsEffectTrack = false;
			m_oReturnValue.m_vOutputID = -1; //Unconnected!
		}

		m_oReturnValue.m_oMediaEffectIDs.clear(); 
	}
	else
		EMDebugger("ERROR! Could not find the specified effect track ID!");

	for(uint32 vSlotIndex = 0; vSlotIndex < opFXTrack -> GetNumberOfSlots(); vSlotIndex++)
	{
		if(opFXTrack -> GetSlot(vSlotIndex) != NULL)
		{
			int32 vID = opFXTrack -> GetSlot(vSlotIndex) -> GetID();
			m_oReturnValue.m_oMediaEffectIDs.push_back(vID);
		}
	}
	
	return static_cast<void*>(&m_oReturnValue);
}

bool MediaInfoCommandGetEffectTrackInfo::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetEffectTrackInfo::IsUndoable()
{
	return false;
}