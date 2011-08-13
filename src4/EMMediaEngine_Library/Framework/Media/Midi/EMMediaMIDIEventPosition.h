#ifndef __EM_MEDIA_MIDI_EVENT_POSITION
#define __EM_MEDIA_MIDI_EVENT_POSITION

#include "EMGlobals.h"

class EMMediaMIDIEvent;
class EMRealtimeMIDIOutputDescriptor;
class EMProjectDataSaver;
class EMProjectDataLoader;
class EMMIDIClip;

class __declspec(dllexport) EMMediaMIDIEventPosition
{
public:
	EMMediaMIDIEventPosition(EMMediaMIDIEvent* p_opMIDIEvent);
	EMMediaMIDIEventPosition(const EMMediaMIDIEventPosition& p_oPosition);
	~EMMediaMIDIEventPosition();

	void SetStart(int64 p_vNativeReferenceTime);
	int64 GetStart() const;
	void SetChannel(uint32 p_vMIDIChannel);
	uint32 GetChannel() const;
	
	void SetDestination(EMRealtimeMIDIOutputDescriptor* p_opDestination);
	EMRealtimeMIDIOutputDescriptor* GetDestination() const;

	void SetMuted(bool p_vIsMuted);
	bool IsMuted() const;

	void SetClip(void* p_opClip);
	void* GetClip() const;
	int64 GetClipStart() const;
	void SetClipStart(int64 p_vStart);
	int32 GetClipID() const;

	bool SaveData(EMProjectDataSaver* p_opSaver);
	bool LoadData(EMProjectDataLoader* p_opLoader);

private:
	EMMediaMIDIEvent* m_opEvent;
	int64 m_vStart;
	uint32 m_vMIDIChannel;
	EMRealtimeMIDIOutputDescriptor* m_opDestination;
	bool m_vIsMuted;
	EMMIDIClip* m_opParentClip;
	friend class EMMediaMIDIEvent;
};

#endif
