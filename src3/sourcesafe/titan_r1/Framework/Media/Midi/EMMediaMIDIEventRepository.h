#ifndef __EM_MEDIA_MIDI_EVENT_REPOSITORY
#define __EM_MEDIA_MIDI_EVENT_REPOSITORY

#include "EMGlobals.h"
#include "EMListener.h"
#include "EMMediaMIDIEventList.h"
#include "EMMediaMIDIEvent.h"
#include "EMMIDIGlobals.h"

class EMMediaMIDITrack;
class EMRealtimeMIDIOutputDescriptor;
class EMProjectDataLoader;
class EMProjectDataSaver;
class EMListenerRepository;

class EMMediaMIDIEventRepository : public EMListener
{
public:
	EMMediaMIDIEventRepository();
	~EMMediaMIDIEventRepository();
	void DiscardAllEvents();
	void SortEvents();
#ifdef _DEBUG
	void Show(); //DEBUGGING!
#endif
	void AdjustToBPM(uint32 p_vNewBPM);
	EMMediaMIDIEvent* FindNoteOnFor(EMMediaMIDIEvent* p_opNoteOffEvent);
	bool PreAllocEvents();
	EMMediaMIDIEvent* AllocEvent();
//	bool Quantize(EMMediaMIDIEvent** p_oppArray, uint32 p_vNum);
	bool SaveData(EMProjectDataSaver* p_opSaver);
	bool LoadData(EMProjectDataLoader* p_opLoader);
	void Store(EMMediaMIDIEvent* p_opEvent);
	void RemoveEvent(EMMediaMIDIEvent* p_opEvent);
	uint64 Size();
	void ResetReadPosition();
	EMMediaMIDIEvent* GetNextEvent(int64 p_vSongTime);
	void MoveEvent(EMMediaMIDIEvent* p_opEvent, int64 p_vNewTime);
	void OnItemAdd(); //Inherited from EMMediaItemContainer. Added by Richard 010821
	void OnItemRemove(); //Inherited from EMMediaItemContainer. Added by Richard 010821
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	EMMediaMIDIEvent* FindPreviousEvent(int64 p_vTime, EMMediaMIDITrack* p_opTrack = NULL, EMMIDIEventType p_eType = EM_MIDI_EVENT_TYPE_UNKNOWN, EMMIDIControlChangeType p_eControlChangeType = EM_MIDI_CONTROL_CHANGE_TYPE_UNKNOWN);

private:
	void BubbleSort(EMMediaMIDIEvent** oppArray, uint64 vNumElements);
	void Swap(EMMediaMIDIEvent** oppArray, uint64 vPositionOne, uint64 vPositionTwo);
	bool m_vHasBeenPrepared;
	uint64 m_vNumPreAllocEvents;
	uint64 m_vPreAllocGetPointer;
	EMMediaMIDIEvent** m_oppPreAllocatedEvents;
	EMMediaMIDIEventList* m_opEventList;
	int64 m_vCurrentBPM;
	EMMediaMIDIEvent* m_opSilentEvent;
	int32 m_vSongSignatureMajor;
	int32 m_vSongSignatureMinor;
	float m_vAutoQuantizeMode;
	string m_oAutoQuantizeMode;
	bool m_vAutoQuantizeEnabled;
	bool m_vAutoQuantizeNoteLengthsToo;
};

#endif
