#ifndef __EM_RECORDING_MIDI_TRACK_REPOSITORY
#define __EM_RECORDING_MIDI_TRACK_REPOSITORY

#include "EMMediaItemContainer.h"
#include "EMMediaMIDITrack.h"

class EMRecordingMIDITrackRepository : public EMMediaItemContainer<EMMediaMIDITrack>
{
public:
	EMRecordingMIDITrackRepository();
	~EMRecordingMIDITrackRepository();

	bool IncomingMIDIEvent(uint64 p_vEventTimeStamp, uint64 p_vEventLengthBytes, void* p_vpEventData);

	void OnItemAdd(); //Inherited from EMMediaItemContainer. Added by Richard 010821
	void OnItemRemove(); //Inherited from EMMediaItemContainer. Added by Richard 010821
	void AddTrack(EMMediaMIDITrack* p_opTrack);
	void RemoveTrack(EMMediaMIDITrack* p_opTrack);

private:

//	EMSemaphore* m_opProtectionSemaphore;
};

#endif