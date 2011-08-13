#include "MediaInfoCommandGetSignalMeterValue.h"

#include "EMMediaSignalMeterRepository.h"
#include "EMMediaSignalMeter.h"
#include "EMMediaProject.h"
#include "EMMediaTrack.h"
#include "EMBeMediaTimer.h"
#include "EMBufferDestination.h"
#include "EMMediaEngine.h"
#include "EMMediaTrackRepository.h"
#include "EMMediaAudioSignalMeter.h"

#include <list>
#include <map>

MediaInfoCommandGetSignalMeterValue::MediaInfoCommandGetSignalMeterValue()
{
}

MediaInfoCommandGetSignalMeterValue::~MediaInfoCommandGetSignalMeterValue()
{
}

void* MediaInfoCommandGetSignalMeterValue::ExecuteE(void* p_upFloatArray, void*, void*)
{
 	float* vpArray = static_cast<float*>(p_upFloatArray);

	if(EMMediaEngine::Instance() -> GetMediaProject() == NULL)
		return NULL;

	if(EMMediaEngine::Instance() -> GetMediaProject() -> GetUsedTracks() == NULL)
		return NULL;

	if(EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks() == NULL)
		return NULL;

	EMMediaEngine::Instance() -> GetMediaProject() -> GetUsedTracks() -> LockContainer();
	EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks() -> LockContainer();

	int32 vTrackNumber = 0;

	list<EMMediaTrack*>::iterator oTrackIterator;
	try
	{
		for(oTrackIterator = EMMediaTrackRepository::m_opTracks -> begin(); oTrackIterator != EMMediaTrackRepository::m_opTracks -> end(); oTrackIterator++)
		{
			EMMediaTrack* opCurrentTrack = (*oTrackIterator);
			if((opCurrentTrack -> GetType() & EM_TYPE_ANY_AUDIO) > 0 && ! opCurrentTrack -> IsObjectDeleted())
			{
				if(opCurrentTrack -> GetSignalMeter() == NULL)
					EMDebugger("ERROR! Audio track has no signal meter!");
				float* vpTrackArray = &(vpArray[vTrackNumber * 5]);
				static_cast<EMMediaAudioSignalMeter*>(opCurrentTrack -> GetSignalMeter()) -> Get(vpTrackArray);
				vTrackNumber++;
			}
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Caught an exception in MediaInfoCommandGetSignalMeterValue");
	}

	EMMediaEngine::Instance() -> GetMediaProject() -> GetUsedTracks() -> UnlockContainer();
	EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks() -> UnlockContainer();

	return NULL;
}

bool MediaInfoCommandGetSignalMeterValue::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetSignalMeterValue::IsUndoable()
{
	return false;
}