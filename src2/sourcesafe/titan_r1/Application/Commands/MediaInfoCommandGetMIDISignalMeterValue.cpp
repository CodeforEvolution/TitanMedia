#include "MediaInfoCommandGetMIDISignalMeterValue.h"

#include "EMMediaSignalMeter.h"
#include "EMMediaProject.h"
#include "EMMediaTrack.h"
#include "EMMediaMIDITrack.h"
#include "EMMediaEngine.h"
#include "EMMediaTrackRepository.h"
#include "EMMediaMIDISignalMeter.h"

#include <list>
#include <map>

///#define EM_OUTPUT_TO_MONITOR "Layla24 1/2 Analog" //Richards Echo-card
//#define EM_OUTPUT_TO_MONITOR "Creative Sound Blaster PCI" //Martin Johanssons
//#define EM_OUTPUT_TO_MONITOR "SB Live! Wave Device" //Richards SB-Live! card
//#define EM_SHOW_AUDIO_CACHE

#ifdef _DEBUG
#ifdef EM_SHOW_AUDIO_CACHE
//Only here for temporary debugging reasons...
#include "EMWinRealtimeAudioOutputDescriptor.h"
#include "EMWinDirectSoundPlayback.h"
#endif
#endif

MediaInfoCommandGetMIDISignalMeterValue::MediaInfoCommandGetMIDISignalMeterValue()
{
}

MediaInfoCommandGetMIDISignalMeterValue::~MediaInfoCommandGetMIDISignalMeterValue()
{
}

void* MediaInfoCommandGetMIDISignalMeterValue::ExecuteE(void* p_upFloatArray, void* , void*)
{
	float* vpMeterValues = static_cast<float*>(p_upFloatArray);

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

	for(oTrackIterator = EMMediaTrackRepository::m_opTracks -> begin(); oTrackIterator != EMMediaTrackRepository::m_opTracks -> end(); oTrackIterator++)
	{
		EMMediaTrack* opCurrentTrack = (*oTrackIterator);
		if((opCurrentTrack -> GetType() & EM_TYPE_MIDI) > 0 && ! opCurrentTrack -> IsObjectDeleted())
		{
			EMMediaMIDITrack* opMIDITrack = static_cast<EMMediaMIDITrack*>(opCurrentTrack);
			if(opMIDITrack -> GetSignalMeter() == NULL)
					EMDebugger("ERROR! MIDI track has no signal meter!");
#ifdef _DEBUG
#ifdef EM_SHOW_AUDIO_CACHE
			EMWinRealtimeAudioOutputDescriptor* opOut = static_cast<EMWinRealtimeAudioOutputDescriptor*>(EMOutputRepository::Instance() -> SearchForName(string(EM_OUTPUT_TO_MONITOR)));
			if(opOut != NULL)
			{
				int32 vMax = opOut -> GetDSPlayback() -> Capacity();
				int32 vUse = opOut -> GetDSPlayback() -> Used();
				float vVal = 1.0 * ((float) vUse) / ((float) vMax);
				vpMeterValues[vTrackNumber] = vVal;
			}
			else
				vpMeterValues[vTrackNumber] = 0.0;
#else
			vpMeterValues[vTrackNumber] = static_cast<EMMediaMIDISignalMeter*>(opMIDITrack -> GetSignalMeter()) -> GetLatestSignal();
#endif
#else
			vpMeterValues[vTrackNumber] = static_cast<EMMediaMIDISignalMeter*>(opMIDITrack -> GetSignalMeter()) -> GetLatestSignal();
#endif
			vTrackNumber++;
		}
	}

	EMMediaEngine::Instance() -> GetMediaProject() -> GetUsedTracks() -> UnlockContainer();
	EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks() -> UnlockContainer();

	return NULL;
}

bool MediaInfoCommandGetMIDISignalMeterValue::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetMIDISignalMeterValue::IsUndoable()
{
	return false;
}