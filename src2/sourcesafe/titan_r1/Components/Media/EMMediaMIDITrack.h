#ifndef __EM_MEDIA_MIDI_TRACK
#define __EM_MEDIA_MIDI_TRACK

#include "EMGlobals.h"
#include "EMMediaTrack.h"
#include "EMListener.h"
#include "EMMediaMIDIEvent.h"
#include "EMMediaMIDIEventList.h"
#include "EMMediaMIDIEventListener.h"

#include <list>

class EMRealtimeMIDIInputDescriptor;
class EMMediaMIDISignalMeter;
class EMProjectDataLoader;
class EMProjectDataSaver;
class EMMediaMIDIEventRepository;
class EMListenerRepository;

class EMMediaMIDITrack : public EMMediaTrack, public EMMediaMIDIEventListener
{
public:
	EMMediaMIDITrack(string p_oName);
	~EMMediaMIDITrack();

	void SetTrackDestination(EMBufferDestination* p_opTrackDataReceiver);

	void StartRecording();
	void StopRecording();
	
	void SetMuteState(EMMuteState p_eFlag);
	void SetSoloed(bool p_vFlag);
	void SetFader(int32 p_vFaderValue);
	void SetPan(int32 p_vPanValue);
	uint32 GetMIDIChannel() const;
	void SetMIDIChannel(uint32 p_vMIDIChannel);
	int32 GetVelocityOffset() const;
	void SetVelocityOffset(int32 p_vOffset);
	int32 GetKeyOffset() const;
	void SetKeyOffset(int32 p_vOffset);

	void SetActive(bool p_vFlag);
	bool IsActive() const;

	void SetArmed(bool p_vFlag);

	bool MIDIEventReceived(EMRealtimeMIDIInputDescriptor* p_opReceiver, uint64 p_vSngTmEventTime, uint64 p_vLengthBytes, void* p_vpData);

	bool SaveData(EMProjectDataSaver* p_opSaver);
	bool LoadData(EMProjectDataLoader* p_opLoader);

	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);

private:

	list<EMMediaMIDIEvent*> m_oNewEvents;
	uint64 m_vRecordingStartFrame;
	uint32 m_vMIDIChannel;
	bool m_vIsActive;
	int32 m_vVelocityOffset;
	int32 m_vKeyOffset;

	EMMediaMIDISignalMeter* m_opMIDISignalMeter;
	EMMediaMIDIEventRepository* m_opEventRepository;

	bool m_vNotesRecordedWithFixedLength; //Is the function for recording MIDI notes with a fixed length enabled?
	int64 m_vFixedMIDINoteLength; //The fixed length (if any) in microseconds of each recorded MIDI note.
};

#endif