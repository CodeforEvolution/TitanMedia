#include "MediaInfoCommandGetEffectTrackID.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaTrackRepository.h"
#include "EMBufferDestination.h"

MediaInfoCommandGetEffectTrackID::MediaInfoCommandGetEffectTrackID()
{
}

void* MediaInfoCommandGetEffectTrackID::ExecuteE(void* p_upMediaTrackID, void*, void*)
{
	int32 vMediaTrackID = *static_cast<int32*>(p_upMediaTrackID);
	
	EMMediaProject* opProject = EMMediaEngine::Instance() -> GetMediaProject();
	EMMediaTrackRepository* opUsed = opProject -> GetUsedTracks();
	EMMediaTrackRepository* opUnused = opProject -> GetUnusedTracks();

	EMMediaTrack* opTrack = NULL;

	opUsed -> LockContainer();
	opTrack = opUsed -> Find(vMediaTrackID);
	opUsed -> UnlockContainer();

	if(opTrack == NULL)
	{
		opUnused -> LockContainer();
		opTrack = opUnused -> Find(vMediaTrackID);
		opUnused -> UnlockContainer();
	}

	if(opTrack != NULL)
	{
		if(opTrack -> GetTrackDestination() != NULL && (opTrack -> GetType() & (EM_TYPE_ANY_AUDIO | EM_TYPE_ANY_VIDEO)) > 0)
		{
			if(opTrack -> GetTrackDestination() -> GetPosition() == EM_EFFECT)
			{
				m_vReturnValue = opTrack -> GetTrackDestination() -> GetID();
				return static_cast<void*>(&m_vReturnValue);
			}
		}
	}
	return NULL;
}

bool MediaInfoCommandGetEffectTrackID::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetEffectTrackID::IsUndoable()
{
	return false;
}