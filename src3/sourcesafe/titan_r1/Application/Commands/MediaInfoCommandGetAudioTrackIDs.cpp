#include "MediaInfoCommandGetAudioTrackIDs.h"

//#include "EMMediaEngine.h"
//#include "EMMediaProject.h"
//#include "EMOutputRepository.h"
#include "EMMediaSignalMeterRepository.h"
#include "EMMediaSignalMeter.h"
#include "EMMediaProject.h"
#include "EMMediaTrack.h"
#include "EMBufferDestination.h"
#include "EMMediaEngine.h"
#include "EMMediaTrackRepository.h"

MediaInfoCommandGetAudioTrackIDs::MediaInfoCommandGetAudioTrackIDs()
{
}

void* MediaInfoCommandGetAudioTrackIDs::ExecuteE(void*, void*, void*)
{
	m_oReturnValue.clear();
	if(EMMediaEngine::Instance() -> GetMediaProject() == NULL)
	{
		eo << "ERROR! There is no project in memory!" << ef;
		return static_cast<void*>(&m_oReturnValue); //TODO: Fix memory leak - Coz Martin can't delete this list!!!
	}

	if(EMMediaEngine::Instance() -> GetMediaProject() -> GetUsedTracks() == NULL)
		EMDebugger("ERROR! No used tracks!");
	if(EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks() == NULL)
		EMDebugger("ERROR! No unused tracks!");

	EMMediaEngine::Instance() -> GetMediaProject() -> GetUsedTracks() -> LockContainer();
	EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks() -> LockContainer();

	list<EMMediaTrack*>::iterator oTrackIterator;
	for(oTrackIterator = EMMediaTrackRepository::m_opTracks -> begin(); oTrackIterator != EMMediaTrackRepository::m_opTracks -> end(); oTrackIterator++)
	{
		EMMediaTrack* opCurrentTrack = (*oTrackIterator);
		if((opCurrentTrack -> GetType() & EM_TYPE_ANY_AUDIO) > 0 && ! opCurrentTrack -> IsObjectDeleted())
			m_oReturnValue.push_back(opCurrentTrack -> GetID());
	}

	EMMediaEngine::Instance() -> GetMediaProject() -> GetUsedTracks() -> UnlockContainer();
	EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks() -> UnlockContainer();

	return static_cast<void*>(&m_oReturnValue);
}

bool MediaInfoCommandGetAudioTrackIDs::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetAudioTrackIDs::IsUndoable()
{
	return false;
}