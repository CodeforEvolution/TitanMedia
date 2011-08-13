#ifndef __EM_MEDIA_MIDI_CLIP
#define __EM_MEDIA_MIDI_CLIP

#include "EMGlobals.h"
#include "EMMediaClip.h" 
#include "EMMediaMIDIEvent.h"

class EMMediaDataBuffer;
class EMMediaTrack;
class EMMediaMIDIEventList;

class EMMIDIClip : public EMMediaClip
{
public:
	EMMIDIClip(uint64 p_vStartFrame, EMMediaTrack* p_opBelongsToTrack);
	EMMIDIClip(EMMediaTrack* p_opOwner);
	~EMMIDIClip();

	virtual EMMediaClip* CloneClip(int64 p_vStartFrame, EMMediaTrack* p_opBelongsToTrack);

	bool InitCheckE();
	EMMediaDataBuffer* GetBuffer() const;
	void DiscardAllEvents();

	void SetTrack(EMMediaTrack* p_opBelongsToTrack);

	void SortEvents();

	void AddEvent(EMMediaMIDIEvent* p_opEvent);
	void RemoveEvent(EMMediaMIDIEvent* p_opEvent);
	void PurgeEvents();
	int64 CountEvents();

	int64 GetActiveLength();
	void SetStart(int64 p_vStart);
	int64 GetStart();
	int64 GetStop();

	void SetMuted(bool p_vAllEventsMuted);
	bool IsMuted() const;
	void SetDeleted(bool p_vFlag);
	void GetAllMIDIEvents(list<EMMediaMIDIEvent*>* p_opListToAddTo);
	void GetActiveMIDIEvents(list<EMMediaMIDIEvent*>* p_opListToAddTo);
	void GetDeletedMIDIEvents(list<EMMediaMIDIEvent*>* p_opListToAddTo);

protected:
	EMMIDIClip();
	EMMediaMIDIEventList* m_opEventList;
	bool m_vAllEventsAreMuted;
};

#endif