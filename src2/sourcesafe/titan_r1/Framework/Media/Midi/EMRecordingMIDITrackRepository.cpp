#include "EMRecordingMIDITrackRepository.h"

#include "EMMediaMIDITrack.h"

EMRecordingMIDITrackRepository::EMRecordingMIDITrackRepository()
	:	EMMediaItemContainer<EMMediaMIDITrack>(EM_TYPE_MIDI)
{

}
EMRecordingMIDITrackRepository::~EMRecordingMIDITrackRepository()
{
}

void EMRecordingMIDITrackRepository::AddTrack(EMMediaMIDITrack* p_opTrack)
{
	LockContainer();
	Add(p_opTrack);
	UnlockContainer();
}

void EMRecordingMIDITrackRepository::RemoveTrack(EMMediaMIDITrack* p_opTrack)
{
	LockContainer();
	Remove(p_opTrack);
	UnlockContainer(); 
}

bool EMRecordingMIDITrackRepository::IncomingMIDIEvent(uint64 p_vEventTimeStamp, uint64 p_vEventLengthBytes, void* p_vpEventData)
{
/*	LockContainer();
	Rewind();
	while(Current() != NULL)
	{
		Current() -> IncomingMIDIEvent(p_vEventTimeStamp, p_vEventLengthBytes, p_vpEventData);
		Next();
	}
	UnlockContainer(); */
	return true;
}

void EMRecordingMIDITrackRepository::OnItemAdd()
{
}

void EMRecordingMIDITrackRepository::OnItemRemove()
{
}

