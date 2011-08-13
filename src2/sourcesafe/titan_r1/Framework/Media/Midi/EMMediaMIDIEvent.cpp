#include "EMMediaMIDIEvent.h"

#include "EMGlobals.h"
#include "EMMediaIDManager.h"
#include "EMMIDIGlobals.h"
#include "EMMediaTrackRepository.h"
#include "EMMediaEngine.h"
#include "EMMediaMIDIEventPosition.h"
#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"
#include "EMMIDIClip.h"

#include <map>

EMMediaMIDIEvent::EMMediaMIDIEvent()
	:	m_vID(-1),
		m_eType(EM_TYPE_MIDI),
		m_vBaseType(0),
		m_vData1(0),
		m_vData2(0),
		m_opPosition(NULL),
		m_vpStart(NULL),
//		m_vpIsMuted(NULL),
		m_vIsDeleted(false),
		m_vDataLength(0),
		m_vIsSelected(false),
		m_opNoteOff(NULL),
		m_vNoteOffID(-1)
{
	m_vID = EMMediaIDManager::MakeID();
	m_opPosition = EM_new EMMediaMIDIEventPosition(this);
	m_vpStart = &(m_opPosition -> m_vStart); //We're a Friend of EMMediaMIDIEventPosition so we can point to it's private member attributes.
//	m_vpIsMuted = &(m_opPosition -> m_vIsMuted); //We're a Friend of EMMediaMIDIEventPosition so we can point to it's private member attributes.
}

EMMediaMIDIEvent::EMMediaMIDIEvent(const EMMediaMIDIEvent& p_oEvent)
	:	m_vID(-1),
		m_eType(EM_TYPE_MIDI),
		m_vBaseType(p_oEvent.m_vBaseType),
		m_vData1(p_oEvent.m_vData1),
		m_vData2(p_oEvent.m_vData2),
		m_opPosition(NULL),
		m_vpStart(NULL),
//		m_vpIsMuted(NULL),
		m_vIsDeleted(p_oEvent.m_vIsDeleted),
		m_vDataLength(p_oEvent.m_vDataLength),
		m_vIsSelected(p_oEvent.m_vIsSelected),
		m_opNoteOff(NULL),
		m_vNoteOffID(-1)
{
	m_vID = EMMediaIDManager::MakeID();
	m_opPosition = EM_new EMMediaMIDIEventPosition(*(p_oEvent.GetPosition()));
	m_vpStart = &(m_opPosition -> m_vStart); //We're a Friend of EMMediaMIDIEventPosition so we can point to it's private member attributes.
//	m_vpIsMuted = &(m_opPosition -> m_vIsMuted); //We're a Friend of EMMediaMIDIEventPosition so we can point to it's private member attributes.
}

EMMediaMIDIEvent::~EMMediaMIDIEvent() 
{
	delete m_opPosition;
	m_vpStart = NULL;
//	m_vpIsMuted = NULL;
}

bool EMMediaMIDIEvent::IsObjectDeleted()
{
	return m_vIsDeleted;
}

void EMMediaMIDIEvent::SetDeleted(bool p_vFlag)
{
	m_vIsDeleted = p_vFlag;
	if(m_opNoteOff != NULL)
		m_opNoteOff -> SetDeleted(p_vFlag);
}

int64* EMMediaMIDIEvent::GetStart()
{
	return m_vpStart;
}

EMMediaMIDIEventPosition* EMMediaMIDIEvent::GetPosition() const
{
	return m_opPosition;
}

bool EMMediaMIDIEvent::Set(int64 p_vSngTmEventTime, uint64 p_vLengthBytes, void* p_vpEventData)
{
	m_opPosition -> SetStart(p_vSngTmEventTime);

	uint8 vType = static_cast<int8*>(p_vpEventData)[0];
	uint8 vChannel = 0;

	if((vType & 240) == EM_MIDI_EVENT_TYPE_NOTE_OFF)			//Note off
	{
		vChannel = (vType & 15);
		m_opPosition -> SetChannel(vChannel);
		vType = EM_MIDI_EVENT_TYPE_NOTE_OFF;
//		eo << "Note OFF on channel " << (int) vChannel << ef;
	}
	else if((vType & 240) == EM_MIDI_EVENT_TYPE_NOTE_ON)		//Note on
	{
		vChannel = (vType & 15);
		m_opPosition -> SetChannel(vChannel);
		vType = EM_MIDI_EVENT_TYPE_NOTE_ON;
		eo << "Note ON with velocity " << (int) m_vData2 << ", key " << (int) m_vData1 << ef;
	}
	else if((vType & 240) == EM_MIDI_EVENT_TYPE_POLYPHONIC_AFTERTOUCH)		//Polyphonic aftertouch
	{
		vChannel = (vType & 15);
		m_opPosition -> SetChannel(vChannel);
		vType = EM_MIDI_EVENT_TYPE_POLYPHONIC_AFTERTOUCH;
//		eo << "Polyphonic aftertouch on channel " << (int) vChannel << ef;
	}
	else if((vType & 240) == EM_MIDI_EVENT_TYPE_CONTROL_CHANGE)		//Control change
	{
		vChannel = (vType & 15);
		m_opPosition -> SetChannel(vChannel);
		vType = EM_MIDI_EVENT_TYPE_CONTROL_CHANGE;
//		eo << "Control change on channel " << (int) vChannel << ef;
		BYTE vControlType = static_cast<int8*>(p_vpEventData)[1];
//		eo << "Control change typ = " << (int) vControlType << ef;
		if(p_vLengthBytes == 3)
		{
			BYTE vValueByte = static_cast<int8*>(p_vpEventData)[2];
//			eo << "Control change value = " << (int) vValueByte << ef;
		}
	}
	else if((vType & 240) == EM_MIDI_EVENT_TYPE_PROGRAM_CHANGE)		//Program change
	{
		vChannel = (vType & 15);
		m_opPosition -> SetChannel(vChannel);
		vType = EM_MIDI_EVENT_TYPE_PROGRAM_CHANGE;
//		eo << "Program change on channel " << (int) vChannel << ef;
	}
	else if((vType & 240) == EM_MIDI_EVENT_TYPE_CHANNEL_AFTERTOUCH)		//Channel aftertouch
	{
		vChannel = (vType & 15);
		m_opPosition -> SetChannel(vChannel);
		vType = EM_MIDI_EVENT_TYPE_CHANNEL_AFTERTOUCH;
//		eo << "Channel aftertouch on channel " << (int) vChannel << ef;
	}
	else if((vType & 240) == EM_MIDI_EVENT_TYPE_PITCH_WHEEL_CONTROL)		//Pitch
	{
		vChannel = (vType & 15);
		m_opPosition -> SetChannel(vChannel);
		vType = EM_MIDI_EVENT_TYPE_PITCH_WHEEL_CONTROL;
//		eo << "Pitch bend on channel " << (int) vChannel << ef;
	}

	//Instead of actually looking at the event, we can just copy it straight into memory starting at the
	//position of our member attribute "m_vType" and then maximum 3 bytes forward.

	static_cast<int8*>(p_vpEventData)[0] = vType;
//	uint8* vpPointerIntoOwnMemory = &m_vBaseType;
//	memcpy(vpPointerIntoOwnMemory, (uint8*) p_vpEventData, p_vLengthBytes);
	m_vDataLength = p_vLengthBytes;
	
	if(m_vDataLength > 0)
		m_vBaseType = ((uint8*) p_vpEventData)[0];
	else
		m_vBaseType = 0;
	
	if(m_vDataLength > 1) 
		m_vData1 = ((uint8*) p_vpEventData)[1];
	else
		m_vData1 = 0;
	
	if(m_vDataLength > 2)
		m_vData2 = ((uint8*) p_vpEventData)[2];
	else
		m_vData2 = 0;
	
	
	return true;
}

uint8 EMMediaMIDIEvent::GetEventType()
{
	if(m_vBaseType == 128 ||
		m_vBaseType == 144 ||
		m_vBaseType == 160 ||
		m_vBaseType == 176 ||
		m_vBaseType == 192 ||
		m_vBaseType == 208 ||
		m_vBaseType == 224)
		return m_vBaseType + m_opPosition -> GetChannel();
	return m_vBaseType;
}

uint8 EMMediaMIDIEvent::GetBaseEventType()
{
	return m_vBaseType;
}

void EMMediaMIDIEvent::Show()
{
	eo << "<EVENT TYPE=" << m_vBaseType << ", DATA1=" << m_vData1 << ", ";
	if(m_vDataLength == 3)
		eo << "DATA2=" << m_vData2;
	eo << ", LEN=" << m_vDataLength << ", CLPID=" << GetClipID() << ", TRKID=" << static_cast<EMMIDIClip*>(m_opPosition -> GetClip()) -> GetTrack() -> GetID() << ef;
}

/*
void EMMediaMIDIEvent::SetMuted(bool p_vIsMuted)
{
	m_opPosition -> SetMuted(p_vIsMuted);
}
*/

bool EMMediaMIDIEvent::IsMuted()
{
	if(GetPosition() -> GetClip() != NULL)
	{
		bool vEventIsMuted = static_cast<EMMediaClip*>(m_opPosition -> GetClip()) -> IsMuted();
		return vEventIsMuted;
	}
	return false;
}

int32 EMMediaMIDIEvent::GetClipID()
{
	//Make sure we don't call a NULL pointer (for instance, if this object was
	//created through the MIDI consumer, then the clip won't be set!)
	if(m_opPosition -> GetClip() == NULL)
		return -1;
	return static_cast<EMMIDIClip*>(m_opPosition -> GetClip()) -> GetID();
}

bool EMMediaMIDIEvent::SaveData(EMProjectDataSaver* p_opSaver)
{
	p_opSaver -> SaveString("EMT");
	p_opSaver -> SaveInt32(m_vID);
	p_opSaver -> SaveInt32(m_eType);
	p_opSaver -> SaveUInt8(m_vDataLength);
	char* vpPointerIntoOwnMemory = reinterpret_cast<char*>(&m_vBaseType);
	p_opSaver -> SaveBytes(vpPointerIntoOwnMemory, m_vDataLength);

	if(m_opNoteOff != NULL)
	{
		//We have a note off pointer!
		p_opSaver -> SaveBool(true);
		p_opSaver -> SaveInt32(m_opNoteOff -> GetID());
	}
	else
	{
		//No note off pointer!
		p_opSaver -> SaveBool(false);
	}

	return m_opPosition -> SaveData(p_opSaver);
}

bool EMMediaMIDIEvent::LoadData(EMProjectDataLoader* p_opLoader)
{
	if(string(p_opLoader -> LoadString()) != string("EMT"))
		return false;
	m_vID = p_opLoader -> LoadInt32();
	m_eType = (EMMediaType) p_opLoader -> LoadInt32();
	m_vDataLength = p_opLoader -> LoadUInt8();
	char* vpPointerIntoOwnMemory = reinterpret_cast<char*>(&m_vBaseType);
	p_opLoader -> LoadBytes(vpPointerIntoOwnMemory, m_vDataLength);

	//If there's a note off id stored, then load it.
	if(p_opLoader -> LoadBool())
		m_vNoteOffID = p_opLoader -> LoadInt32();

	delete m_opPosition;
	m_opPosition = EM_new EMMediaMIDIEventPosition(this);
	if(! m_opPosition -> LoadData(p_opLoader))
		return false;
	m_vpStart = &(m_opPosition -> m_vStart); //We're a Friend of EMMediaMIDIEventPosition so we can point to it's private member attributes.
	static_cast<EMMIDIClip*>(m_opPosition -> GetClip()) -> AddEvent(this);
	static_cast<EMMIDIClip*>(m_opPosition -> GetClip()) -> SortEvents();
	return true;
}
