#include "MediaInfoCommandGetMIDITrackIDs.h"

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

MediaInfoCommandGetMIDITrackIDs::MediaInfoCommandGetMIDITrackIDs()
{
}

void* MediaInfoCommandGetMIDITrackIDs::ExecuteE(void*, void*, void*)
{
	m_oReturnValue.clear();
	if(EMMediaEngine::Instance() -> GetMediaProject() == NULL)
	{
		eo << "ERROR! There is no project in memory!" << ef;
		return static_cast<void*>(&m_oReturnValue); //TODO: Fix memory leak - Coz Martin can't delete this list!!!
	}

	EMMediaEngine::Instance() -> GetMediaProject() -> GetUsedTracks() -> LockContainer();
	EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks() -> LockContainer();

	list<EMMediaTrack*>::iterator oTrackIterator;
	for(oTrackIterator = EMMediaTrackRepository::m_opTracks -> begin(); oTrackIterator != EMMediaTrackRepository::m_opTracks -> end(); oTrackIterator++)
	{
		EMMediaTrack* opCurrentTrack = (*oTrackIterator);
		if((opCurrentTrack -> GetType() & EM_TYPE_MIDI) > 0 && ! opCurrentTrack -> IsObjectDeleted())
			m_oReturnValue.push_back(opCurrentTrack -> GetID());
	}

	EMMediaEngine::Instance() -> GetMediaProject() -> GetUsedTracks() -> UnlockContainer();
	EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks() -> UnlockContainer();

/*	eo << "#################################################" << ef;
	eo << "MediaInfoCommandGetMIDITrackIDs returning ";
	list<int32>::const_iterator i;
	for(i = m_oReturnValue.begin(); i != m_oReturnValue.end(); i++)
		eo << "[" << (int) *i << "] ";
	eo << ef;
	eo << "#################################################" << ef; */

	return static_cast<void*>(&m_oReturnValue);
}

bool MediaInfoCommandGetMIDITrackIDs::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetMIDITrackIDs::IsUndoable()
{
	return false;
}