#ifndef __EM_MEDIA_MIDI_EVENT_LIST_CONTAINER
#define __EM_MEDIA_MIDI_EVENT_LIST_CONTAINER

#include "EMGlobals.h"

#include "EMMediaMIDIEvent.h"

class EMMediaMIDIEventItem
{
public:
	EMMediaMIDIEventItem(EMMediaMIDIEvent* p_opEvent, EMMediaMIDIEventItem* p_opPrevious = NULL) { m_opEvent = p_opEvent; m_opNext = NULL; m_opPrevious = p_opPrevious; };
	~EMMediaMIDIEventItem() 
	{ 
		delete m_opEvent; 
		delete m_opNext; 
	};
	EMMediaMIDIEventItem* m_opNext;
	EMMediaMIDIEventItem* m_opPrevious;
	EMMediaMIDIEvent* m_opEvent;
};


#endif

#ifndef __EM_MEDIA_MIDI_EVENT_LIST
#define __EM_MEDIA_MIDI_EVENT_LIST

#include "EMGlobals.h"

class EMMediaMIDIEvent;
class EMProjectDataSaver;
class EMSemaphore;

class EMMediaMIDIEventList
{
public:
	EMMediaMIDIEventList();
	~EMMediaMIDIEventList();
	void Lock();
	void Unlock();

	void Store(EMMediaMIDIEvent* p_opEvent);
	void Remove(EMMediaMIDIEvent* p_opEvent);
	void Sort();
	void Recalculate();
	void Reset();
	void Clear();
	uint64 Size();
	
	void MoveEvent(EMMediaMIDIEvent* p_opEvent, int64 p_vNewTime);
	bool SaveData(EMProjectDataSaver* p_opSaver);
	EMMediaMIDIEventItem* FindPreviousTo(EMMediaMIDIEvent* p_opEvent);

	EMMediaMIDIEvent* operator [](int64 p_vTime);
	EMMediaMIDIEvent* First();
	EMMediaMIDIEvent* Last();
	EMMediaMIDIEventItem* FirstItem();
	EMMediaMIDIEventItem* LastItem();

#ifdef _DEBUG	
	void Show();
#endif

private:
	EMMediaMIDIEventItem* m_opFirst;
	EMMediaMIDIEventItem* m_opReadPosition;
	EMMediaMIDIEventItem* m_opWritePosition;
	EMSemaphore* m_opListProtectionSemaphore;
	bool m_vLocked;
	int64 m_vNumItems;
};

#endif