#ifndef __EM_MEDIA_MIDI_EVENT
#define __EM_MEDIA_MIDI_EVENT

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

class EMMediaMIDIEventPosition;
class EMProjectDataLoader;
class EMProjectDataSaver;

class __declspec(dllexport) EMMediaMIDIEvent
{
public:
	EMMediaMIDIEvent();
	EMMediaMIDIEvent(const EMMediaMIDIEvent& p_oEvent);
	~EMMediaMIDIEvent();

	bool Set(int64 p_vSngTmEventTime, uint64 p_vLengthBytes, void* p_vpEventData);
	EMMediaType GetType() const { return m_eType; };
	int32 GetID() const { return m_vID; };

	EMMediaMIDIEventPosition* GetPosition() const; 

//	void SetMuted(bool p_vIsMuted);
	bool IsMuted();
	uint8 GetEventType();
	uint8 GetBaseEventType();
	void Show(); //DEBUGGING!

	int64* GetStart();

	uint8 m_vBaseType;		//Note ON, Note OFF, Wheel, Controller, ...
	uint8 m_vData1;			//Data field, for events other than NoteONs..
	uint8 m_vData2;			//Data field, for events other than NoteONs..

	int64* m_vpStart;		//I know some guys (including me :-)) will have a problem with this.
							//But... I wanted to keep the position info separated from the actual event information,
							//and still have "fast" access to the time-variable without having to do twelve method calls
							//and memory copies... . So, I made the Position-class have the Event class as a friend, 
							//and then set this pointer to point to the position-object's
							//private member attribue "m_vStart"... Hehe.. *hacking* .. :-)

	bool IsObjectDeleted();
	void SetDeleted(bool p_vFlag);

	EMMediaMIDIEvent* m_opNoteOff;	//If this event is a note ON, then this will point to its corresponding note OFF event. Otherwize it will be NULL.

	bool SaveData(EMProjectDataSaver* p_opSaver);
	bool LoadData(EMProjectDataLoader* p_opLoader);

	bool m_vIsSelected;		//For performance reasons, store this GUI-related information in here, in the event.
	uint8 m_vDataLength;	//The number of bytes of the MIDI data (usually 2 or 3)

	int32 GetClipID();

	EMMediaMIDIEvent* m_opClone; //Only for clone purposes. Don't use!
	int32 m_vNoteOffID;			//Only used for loading! Don't use!
private:
//	bool* m_vpIsMuted;		//Same arguments here! :-)
	bool m_vIsDeleted;
	int32 m_vID;			//Unique ID number of event/instance!
	EMMediaType m_eType;	//EM_TYPE_MIDI is only valid value
	EMMediaMIDIEventPosition* m_opPosition;
};

#endif
