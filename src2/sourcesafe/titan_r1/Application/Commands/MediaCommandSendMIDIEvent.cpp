#include "MediaCommandSendMIDIEvent.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMMediaEngine.h"
#include "EMMediaMIDIEvent.h"
#include "EMMediaMIDIEventPosition.h"
#include "EMMediaMIDIEventRepository.h"
#include "EMMediaMIDITrack.h"
#include "EMMediaProject.h"
#include "EMMediaTrackRepository.h"
#include "EMMIDIClip.h"
#include "EMMIDIGlobals.h"
#include "EMRealtimeMIDIOutputDescriptor.h"
#include "EMWinDXMIDIProducer.h"

MediaCommandSendMIDIEvent::MediaCommandSendMIDIEvent()
{
}

void* MediaCommandSendMIDIEvent::ExecuteE(void* p_upMediaTrackID, void* p_upKeyNumber, void* p_upVelocity)
{
	int32 vTrackID = *static_cast<int32*>(p_upMediaTrackID);
	int8 vKeyNumber = *static_cast<int8*>(p_upKeyNumber);
	int8 vVelocity = *static_cast<int8*>(p_upVelocity);

	EMMIDIEventType eEventType = (vVelocity == 0 ? EM_MIDI_EVENT_TYPE_NOTE_OFF : EM_MIDI_EVENT_TYPE_NOTE_ON);

	EMMediaMIDITrack* opTrack = NULL;
	EMMediaEngine::Instance() -> GetMediaProject() -> GetUsedTracks() -> LockContainer();
	opTrack = static_cast<EMMediaMIDITrack*>(EMMediaEngine::Instance() -> GetMediaProject() -> GetUsedTracks() -> Find(vTrackID));
	EMMediaEngine::Instance() -> GetMediaProject() -> GetUsedTracks() -> UnlockContainer();

	if(opTrack == NULL)
	{
		EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks() -> LockContainer();
		opTrack = static_cast<EMMediaMIDITrack*>(EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks() -> Find(vTrackID));
		EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks() -> UnlockContainer();
	}

	if(opTrack == NULL)
		return NULL;

	uint32 vChannel = opTrack -> GetMIDIChannel();

	EMRealtimeMIDIOutputDescriptor* opOutputDescriptor = static_cast<EMRealtimeMIDIOutputDescriptor*>(opTrack -> GetTrackDestination());
	int64 vTime = 0;
	int8 vpData[3];
	vpData[0] = static_cast<int8>(eEventType) + vChannel;
	vpData[1] = vKeyNumber;
	vpData[2] = vVelocity;

	EMWinDXMIDIProducer::Instance() -> SendEvent(opOutputDescriptor, vTime, 3, vpData);
	return NULL;
}

bool MediaCommandSendMIDIEvent::RequiresParameters()
{
	return true;
}

bool MediaCommandSendMIDIEvent::IsUndoable()
{
	return false;
}