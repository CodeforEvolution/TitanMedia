#include "EMMediaMIDITrack.h"

#include "EMMediaTrack.h"
#include "EMMediaMIDIEvent.h"
#include "EMMIDIClip.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaTimer.h"
#include "EMMediaUtility.h"
#include "EMMediaClipRepository.h"
#include "EMCommandRepository.h"
#include "CommandIDs.h"
#include "EMMediaMIDIEventRepository.h"
#include "EMMediaMIDIEventPosition.h"
#include "EMRealtimeMIDIOutputDescriptor.h"
#include "EMRealtimeMIDIInputDescriptor.h"
#include "EMMediaMIDISignalMeter.h"
#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"
#include "EMMediaTrackRepository.h"
#include "EMMIDIGlobals.h"
#include "EMListenerRepository.h"
#include "EMMediaDebugLog.h"
 
#include "EMWinRealtimeMIDIOutputDescriptor.h" //TODO: Make non-native!!!!!!!!!!!!!!!!!!!!!11
#include "EMWinDXMIDIProducer.h" //TODO: Make non-native!!!!!!!!!!!!!!!!!!!!!11
#include "EMWinDXMIDIConsumer.h" //TODO: Make non-native!!!!!!!!!!!!!!!!!!!!!11
//#include "EMMediaClipRepository.h"

EMMediaMIDITrack::EMMediaMIDITrack(string p_oName)
	:	EMMediaTrack(EM_TYPE_MIDI, p_oName, 0), 
		EMMediaMIDIEventListener(),
		m_vMIDIChannel(0),
		m_vIsActive(false),
		m_vVelocityOffset(0),
		m_vKeyOffset(0),
		m_opMIDISignalMeter(NULL),
		m_opEventRepository(NULL),
		m_vNotesRecordedWithFixedLength(false),
		m_vFixedMIDINoteLength(0)
{
	m_opMIDISignalMeter = static_cast<EMMediaMIDISignalMeter*>(EMMediaTrack::m_opSignalMeter);
	m_opEventRepository = EMMediaEngine::Instance() -> GetMediaProject() -> GetMIDIEventRepository();
	//m_vNotesRecordedWithFixedLength = *static_cast<bool*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(EM_SETTING_MIDI_RECORDING_FIXED_LENGTH_ENABLED));
	//m_vFixedMIDINoteLength = *static_cast<int64*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(EM_SETTING_MIDI_RECORDING_FIXED_LENGTH));
}

EMMediaMIDITrack::~EMMediaMIDITrack()
{
	m_opMIDISignalMeter = NULL;
}

void EMMediaMIDITrack::SetTrackDestination(EMBufferDestination* p_opTrackDataReceiver)
{
	EMMediaTrack::SetTrackDestination(p_opTrackDataReceiver);

	EMMediaClipRepository* opClips = EMMediaEngine::Instance() -> GetMediaProject() -> GetMIDIClipRepository();
	opClips -> LockContainer();
	opClips -> Rewind();
	while(opClips -> Current() != NULL)
	{
		if(opClips -> Current() -> GetTrack() == this)
		{
			opClips -> Current() -> SetDestination(p_opTrackDataReceiver);
			list<EMMediaMIDIEvent*> oList;
			static_cast<EMMIDIClip*>(opClips -> Current()) -> GetAllMIDIEvents(&oList);
			while(oList.size() > 0)
			{
				oList.front() -> GetPosition() -> SetDestination(static_cast<EMRealtimeMIDIOutputDescriptor*>(p_opTrackDataReceiver));
				oList.pop_front();
			}
		}
		opClips -> Next();
	}
	opClips -> UnlockContainer();
}

void EMMediaMIDITrack::SetActive(bool p_vFlag)
{
	m_vIsActive = p_vFlag;
	if(m_vIsActive)
	{
	}
	else
	{
	}
}

bool EMMediaMIDITrack::IsActive() const
{
	return m_vIsActive;
}

void EMMediaMIDITrack::SetArmed(bool p_vFlag)
{
	EMMediaTrack::SetArmed(p_vFlag);
	if(p_vFlag)
	{
		if(GetTrackDestination() != NULL)
			static_cast<EMWinRealtimeMIDIOutputDescriptor*>(GetTrackDestination()) -> GetProducer() -> SetActiveMIDIRoutingOutputChannel(GetMIDIChannel());
		else
			MessageBox(NULL, "Your system does not seem to have any MIDI outputs.\nArming a MIDI track is useless.", "Warning!", MB_OK);
	}
	else
	{
	}
}

void EMMediaMIDITrack::SetFader(int32 p_vFaderValue)
{
	EMMediaTrack::SetFader(p_vFaderValue);
	uint8 vpData[3];
	uint16 vWord = 0;

	float vValue = static_cast<float>(GetFader());
	vValue = (vValue / 127.0) * 0x7F7F;

//	vWord = static_cast<uint16>(vValue);
//	vpData[0] = EM_MIDI_EVENT_TYPE_CONTROL_CHANGE;
//	vpData[1] = EM_MIDI_CONTROL_CHANGE_TYPE_CHANNEL_VOLUME_LSB;
//	vpData[2] = p_vFaderValue; //(vWord & 0x7F);
//	EMWinDXMIDIProducer::Instance() -> SendEvent(static_cast<EMRealtimeMIDIOutputDescriptor*>(GetTrackDestination()), 0, 3, &vpData, GetMIDIChannel());

//	vWord = static_cast<uint16>(vValue);
	vpData[0] = EM_MIDI_EVENT_TYPE_CONTROL_CHANGE;
	vpData[1] = EM_MIDI_CONTROL_CHANGE_TYPE_CHANNEL_VOLUME_MSB;
	vpData[2] = static_cast<uint8>(p_vFaderValue); //(vWord & 0x3F80) >> 7;
	EMWinDXMIDIProducer::Instance() -> SendEvent(static_cast<EMRealtimeMIDIOutputDescriptor*>(GetTrackDestination()), 0, 3, &vpData, GetMIDIChannel());
}

void EMMediaMIDITrack::SetPan(int32 p_vPanValue)
{
	EMMediaTrack::SetPan(p_vPanValue);
	uint8 vpData[3];
	uint16 vWord = 0;

	float vValue = static_cast<float>(GetPan() + 63);
	vValue = (vValue / 127.0) * 0x7F7F;

	vWord = static_cast<uint16>(vValue);
	vpData[0] = EM_MIDI_EVENT_TYPE_CONTROL_CHANGE;
	vpData[1] = EM_MIDI_CONTROL_CHANGE_TYPE_BALANCE_MSB;
	vpData[2] = /*static_cast<uint8>(p_vPanValue + 63); */(vWord & 0x3F80) >> 7;
	EMWinDXMIDIProducer::Instance() -> SendEvent(static_cast<EMRealtimeMIDIOutputDescriptor*>(GetTrackDestination()), 0, 3, &vpData, GetMIDIChannel());

//	vWord = static_cast<uint16>(vValue);
//	vpData[0] = EM_MIDI_EVENT_TYPE_CONTROL_CHANGE;
//	vpData[1] = 42; //EM_MIDI_CONTROL_CHANGE_TYPE_BALANCE_LSB;
//	vpData[2] = (vWord & 0x7F);
//	EMWinDXMIDIProducer::Instance() -> SendEvent(static_cast<EMRealtimeMIDIOutputDescriptor*>(GetTrackDestination()), 0, 3, &vpData, GetMIDIChannel()); 
}

uint32 EMMediaMIDITrack::GetMIDIChannel() const
{
	return m_vMIDIChannel;
}

void EMMediaMIDITrack::SetMIDIChannel(uint32 p_vMIDIChannel)
{
	m_vMIDIChannel = p_vMIDIChannel;
	if(GetTrackDestination() == NULL)
	{
		MessageBox(NULL, "Can't set MIDI channel for a MIDI track with no output!\n\n\nEither the MIDI functionality has been disabled in this\nversion of Titan, or you don't have a MIDI interface \ninstalled on your system!\n\nIf you've been informed that the MIDI functionality IS enabled\nin this version, and you DO have a MIDI interface installed, \nthen please report this as a bug! \n\nThank you!\n//Elegant Media development team.\n", "Error!", MB_OK);
		return;
	}
	if(m_vIsActive)
		static_cast<EMWinRealtimeMIDIOutputDescriptor*>(GetTrackDestination()) -> GetProducer() -> SetActiveMIDIRoutingOutputChannel(m_vMIDIChannel);
	else
		eo << "Just setting the track's MIDI channel! Track not ACTIVE" << ef;
}

int32 EMMediaMIDITrack::GetVelocityOffset() const
{
	return m_vVelocityOffset;
}

void EMMediaMIDITrack::SetVelocityOffset(int32 p_vOffset)
{
	if(p_vOffset > 127)
		p_vOffset = 127;
	if(p_vOffset < -127)
		p_vOffset = -127;
	m_vVelocityOffset = p_vOffset;
}

int32 EMMediaMIDITrack::GetKeyOffset() const
{
	return m_vKeyOffset;
}

void EMMediaMIDITrack::SetKeyOffset(int32 p_vOffset)
{
	if(p_vOffset > 127)
		p_vOffset = 127;
	if(p_vOffset < -127)
		p_vOffset = -127;
	m_vKeyOffset = p_vOffset;
}

void EMMediaMIDITrack::StartRecording()
{
	m_oNewEvents.clear();
}

void EMMediaMIDITrack::StopRecording()
{
	EMMIDIClip* opNewMIDIClip = NULL;

	EMMediaMIDIEventRepository* opRepository = m_opEventRepository;
	int32 vMediaClipID = 0;
	int32 vMediaTrackID = 0;
	int64 vRecordingStartTime = 0;

	list<EMMediaMIDIEvent*> oNoteOns;

	while(m_oNewEvents.size() > 0)
	{
		EMMediaMIDIEvent* opNewEvent = m_oNewEvents.front();

		if(opNewEvent -> m_vBaseType == EM_MIDI_EVENT_TYPE_NOTE_ON)
			oNoteOns.push_back(opNewEvent);

		m_oNewEvents.pop_front();
		int64 vEventStart = 0;

		try
		{
			if(opNewMIDIClip == NULL)
			{
				uint64 vEventStart = *(opNewEvent -> m_vpStart);
				uint64 vClipStart = EMMediaUtility::Instance() -> TimeToFrames(vEventStart, EMMediaUtility::Instance() -> GetSystemAudioFormat());
				
				opNewMIDIClip = EM_new EMMIDIClip(vClipStart, this); //The first clip's starting time will be equal to the starting time (in frames) of the first event in this recording session!
				opNewMIDIClip -> SetDestination(GetClipDataDestination());
				if(! opNewMIDIClip -> InitCheckE())
				{
					EMMediaDebugLog::Instance() -> Log("ERROR! Failed to initialize MediaClip in EMMediaMIDITrack::StopRecording");
					continue;
				}
				opNewMIDIClip -> SetStart(vClipStart);

				vMediaClipID = opNewMIDIClip -> GetID();
				vMediaTrackID = GetID();

				//This is the starting time of the clip!
				vRecordingStartTime = vEventStart; //*(opNewEvent -> m_vpStart), EMMediaUtility::Instance() -> GetSystemAudioFormat()); 
			}

			//Subtract the clip's starting time (in frames) so we refer to the clip when talking about event starting times!
			vEventStart = *(opNewEvent -> m_vpStart); // - vRecordingStartTime; 

			opNewEvent -> GetPosition() -> SetStart(vEventStart);
			opNewEvent -> GetPosition() -> SetDestination(static_cast<EMRealtimeMIDIOutputDescriptor*>(GetTrackDestination()));
			opNewEvent -> GetPosition() -> SetClip(static_cast<void*>(opNewMIDIClip));	//Have to have void* instead of EMMIDIClip* because EMMediaMIDIEvent is exported 
																						//and can't include MIDIClip as parameter or return value in any method

			if(opNewEvent -> m_vBaseType != EM_MIDI_EVENT_TYPE_NOTE_ON)
			{	//Don't add NOTE ON:s until we've got their corresponding NOTE OFF event!
				opNewMIDIClip -> AddEvent(opNewEvent);
				opRepository -> Store(opNewEvent);
			}
		}
		catch(...)
		{
			EMDebugger("Exception while creating new clip for the new MIDI events, and all that sortof stuff...");
		}

		if(opNewEvent -> m_vBaseType == EM_MIDI_EVENT_TYPE_NOTE_ON && opNewEvent -> m_vData2 == 0)
			EMDebugger("ERROR! Illegal event type received!\n\nNote ON with velocity 0 received, despite the conversion earlier that supposedly eliminates these things...!");

		if(opNewEvent -> m_vBaseType == EM_MIDI_EVENT_TYPE_NOTE_OFF)
		{
			EMMediaMIDIEvent* opNoteOnCandidate = NULL;
			int64 vMinimumDistance = -1;

			list<EMMediaMIDIEvent*>::const_iterator oEvent;
			for(oEvent = oNoteOns.begin(); oEvent != oNoteOns.end(); oEvent++)
			{
				EMMediaMIDIEvent* opCurrentNoteOn = (*oEvent);
				EMMediaMIDIEvent* opCurrentNoteOff = opNewEvent;

				if(opCurrentNoteOn -> m_vData1 == opCurrentNoteOff -> m_vData1 && 
					*(opCurrentNoteOn -> m_vpStart) <= *(opCurrentNoteOff -> m_vpStart))
				{
					int64 vCurrentDistance = *(opCurrentNoteOff -> m_vpStart) - *(opCurrentNoteOn -> m_vpStart);
					if(vCurrentDistance < vMinimumDistance || vMinimumDistance == -1)
					{
						opNoteOnCandidate = opCurrentNoteOn;
						vMinimumDistance = vCurrentDistance;
					}
				}
			}
			if(opNoteOnCandidate != NULL && vMinimumDistance != -1)
			{
				oNoteOns.remove(opNoteOnCandidate);
				opNoteOnCandidate -> m_opNoteOff = opNewEvent;
				opNewMIDIClip -> AddEvent(opNoteOnCandidate);

				opRepository -> Store(opNoteOnCandidate);

				//Handle the function for recording MIDI notes with a fixed length (if enabled)
				//Put this AFTER the note has been stored in the repository. Coz' when it's added
				//it's (the NOTE OFF's) start time is also quantized (possibly).. And we need the
				//fixed-length of a note to override that quantization - i.e change the note's 
				//start time one more time after quantize has been done.
				if(m_vNotesRecordedWithFixedLength && m_vFixedMIDINoteLength > 0)
					*(opNoteOnCandidate -> m_opNoteOff -> m_vpStart) = *(opNoteOnCandidate -> m_vpStart) + m_vFixedMIDINoteLength;
			}
			else
				EMMediaDebugLog::Instance() -> Log("ERROR! Found a note on event that lacks a corresponding note off, in EMMediaMIDITrack::StopRecording. Will just ignore it!");
		}
	}

	if(opNewMIDIClip != NULL)
	{
		try
		{
			EMMediaClipRepository* opMIDIClipRepository = EMMediaEngine::Instance() -> GetMediaProject() -> GetMIDIClipRepository();
			opNewMIDIClip -> SortEvents();
			opMIDIClipRepository -> LockContainer();
			opMIDIClipRepository -> Add(opNewMIDIClip);
			opMIDIClipRepository -> UnlockContainer();
		}
		catch(...)
		{
			EMDebugger("Exception while getting media clip repository, locking/unlocking it and adding the new MediaClip to it!");
		}

		try
		{
			int32 vGTrackIndex = *static_cast<int32*>(EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_GET_TRACK_GUI_INDEX, &vMediaTrackID, NULL, NULL));
			EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_ADD_G_MIDI_CLIP, &vMediaClipID, &vGTrackIndex, &vRecordingStartTime);
		}
		catch(...)
		{
			EMDebugger("Exception caught from calls to COMMAND_GET_TRAC_GUI_INDEX and COMMAND_ADD_G_MIDI_CLIP");
		}
	}

	
	try
	{
		opRepository -> SortEvents();
	}
	catch(...)
	{
		EMDebugger("Exception while sorting new, recorded events!");
	}
}

bool EMMediaMIDITrack::MIDIEventReceived(EMRealtimeMIDIInputDescriptor* p_opReceiver, uint64 p_vSngTmEventTime, uint64 p_vLengthBytes, void* p_vpData)
{
	m_opMIDISignalMeter -> RegisterSignal((uint8) p_vLengthBytes, static_cast<const uint8*>(p_vpData));
	if(IsArmed() && EMMediaTimer::Instance() -> GetState() == EM_RECORDING)
	{
		EMMediaMIDIEvent* opNewMIDIEvent = m_opEventRepository -> AllocEvent(); ////EM_new EMMediaMIDIEvent();
		opNewMIDIEvent -> Set(p_vSngTmEventTime, p_vLengthBytes, p_vpData); //uS from rec-start time, length, data
		m_oNewEvents.push_back(opNewMIDIEvent);
		return true;
	}
	return false;
}

void EMMediaMIDITrack::SetMuteState(EMMuteState p_eFlag)
{
	EMMediaTrack::SetMuteState(p_eFlag);
	EMMediaClipRepository* opClips = EMMediaEngine::Instance() -> GetMediaProject() -> GetMIDIClipRepository();
	opClips -> LockContainer();
	opClips -> Rewind();
	while(opClips -> Current() != NULL)
	{
		if(opClips -> Current() -> GetTrack() -> GetID() == GetID() && (opClips -> Current() -> GetType() & EM_TYPE_MIDI) > 0)
		{
			EMMIDIClip* opMIDIClip = static_cast<EMMIDIClip*>(opClips -> Current());
			if(p_eFlag == EM_MUTE_ON || p_eFlag == EM_MUTE_ON_BY_SOLO)
				opMIDIClip -> SetMuted(true);
			else
				opMIDIClip -> SetMuted(false);
		}
		opClips -> Next();
	}
	opClips -> UnlockContainer();
}

void EMMediaMIDITrack::SetSoloed(bool p_vFlag)
{
	EMMediaTrack::SetSoloed(p_vFlag);
	EMMediaClipRepository* opClips = EMMediaEngine::Instance() -> GetMediaProject() -> GetMIDIClipRepository();
	opClips -> LockContainer();
	opClips -> Rewind();
	while(opClips -> Current() != NULL)
	{
		if(opClips -> Current() -> GetTrack() -> GetID() == GetID() && (opClips -> Current() -> GetType() & EM_TYPE_MIDI) > 0)
		{
			EMMIDIClip* opMIDIClip = static_cast<EMMIDIClip*>(opClips -> Current());
			if(p_vFlag == true)
				opMIDIClip -> SetMuted(false);
		}
		opClips -> Next();
	}
	opClips -> UnlockContainer();
}

bool EMMediaMIDITrack::SaveData(EMProjectDataSaver* p_opSaver)
{
	p_opSaver -> SaveString("EMT");
	EMMediaTrack::SaveData(p_opSaver);
	p_opSaver -> SaveBool(m_vIsActive);
	p_opSaver -> SaveUInt32(m_vMIDIChannel);
	p_opSaver -> SaveInt32(m_vVelocityOffset);
	p_opSaver -> SaveInt32(m_vKeyOffset);
	return true;
}

bool EMMediaMIDITrack::LoadData(EMProjectDataLoader* p_opLoader)
{
	if(string(p_opLoader -> LoadString()) != string("EMT"))
	{
		EMDebugger("ERROR! Error in project file!");
		return false;
	}
	EMMediaTrack::LoadData(p_opLoader);
	m_vIsActive = p_opLoader -> LoadBool();
	m_vMIDIChannel = p_opLoader -> LoadUInt32();
	m_vVelocityOffset = p_opLoader -> LoadInt32();
	m_vKeyOffset = p_opLoader -> LoadInt32();
	return true;
}

bool EMMediaMIDITrack::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	EMMediaTrack::MessageReceived(p_opSender, p_vMessage);
	switch(p_vMessage)
	{
		case EM_MESSAGE_TIME_WAS_SEEKED:
		{
/*			SetFader(GetFader()); //Causes a MIDI event to be sent out to set the volume
			SetPan(GetPan()); //Causes a MIDI event to be sent out to set the pan
			
			EMMediaMIDIEvent* opEvent = NULL;
			opEvent = EMMediaEngine::Instance() -> GetMediaProject() -> GetMIDIEventRepository() -> FindPreviousEvent(EMMediaTimer::Instance() -> NowTime(), this, EM_MIDI_EVENT_TYPE_PITCH_WHEEL_CONTROL);
			if(opEvent != NULL)
				EMWinDXMIDIProducer::Instance() -> SendEvent(static_cast<EMRealtimeMIDIOutputDescriptor*>(GetTrackDestination()), 0, opEvent -> m_vDataLength, &(opEvent -> m_vBaseType), GetMIDIChannel());

			opEvent = EMMediaEngine::Instance() -> GetMediaProject() -> GetMIDIEventRepository() -> FindPreviousEvent(EMMediaTimer::Instance() -> NowTime(), this, EM_MIDI_EVENT_TYPE_PROGRAM_CHANGE);
			if(opEvent != NULL)
				EMWinDXMIDIProducer::Instance() -> SendEvent(static_cast<EMRealtimeMIDIOutputDescriptor*>(GetTrackDestination()), 0, opEvent -> m_vDataLength, &(opEvent -> m_vBaseType), GetMIDIChannel());

			opEvent = EMMediaEngine::Instance() -> GetMediaProject() -> GetMIDIEventRepository() -> FindPreviousEvent(EMMediaTimer::Instance() -> NowTime(), this, EM_MIDI_EVENT_TYPE_CONTROL_CHANGE, EM_MIDI_CONTROL_CHANGE_TYPE_MODULATION_WHEEL_MSB);
			if(opEvent != NULL)
				EMWinDXMIDIProducer::Instance() -> SendEvent(static_cast<EMRealtimeMIDIOutputDescriptor*>(GetTrackDestination()), 0, opEvent -> m_vDataLength, &(opEvent -> m_vBaseType), GetMIDIChannel());
*/
			break; 
		}
		case EM_MESSAGE_BEGIN_PLAYORRECORD:
		{
/*			SetFader(GetFader()); //Causes a MIDI event to be sent out to set the volume
			SetPan(GetPan()); //Causes a MIDI event to be sent out to set the pan
			
			EMMediaMIDIEvent* opEvent = NULL;
			opEvent = EMMediaEngine::Instance() -> GetMediaProject() -> GetMIDIEventRepository() -> FindPreviousEvent(EMMediaTimer::Instance() -> NowTime(), this, EM_MIDI_EVENT_TYPE_PITCH_WHEEL_CONTROL);
			if(opEvent != NULL)
				EMWinDXMIDIProducer::Instance() -> SendEvent(static_cast<EMRealtimeMIDIOutputDescriptor*>(GetTrackDestination()), 0, opEvent -> m_vDataLength, &(opEvent -> m_vBaseType), GetMIDIChannel());
			else
				; //Initiate with default value (0 ?)
			opEvent = EMMediaEngine::Instance() -> GetMediaProject() -> GetMIDIEventRepository() -> FindPreviousEvent(EMMediaTimer::Instance() -> NowTime(), this, EM_MIDI_EVENT_TYPE_PROGRAM_CHANGE);
			if(opEvent != NULL)
				EMWinDXMIDIProducer::Instance() -> SendEvent(static_cast<EMRealtimeMIDIOutputDescriptor*>(GetTrackDestination()), 0, opEvent -> m_vDataLength, &(opEvent -> m_vBaseType), GetMIDIChannel());
			else
				; //Initiate with default value (0 ?)
			opEvent = EMMediaEngine::Instance() -> GetMediaProject() -> GetMIDIEventRepository() -> FindPreviousEvent(EMMediaTimer::Instance() -> NowTime(), this, EM_MIDI_EVENT_TYPE_CONTROL_CHANGE, EM_MIDI_CONTROL_CHANGE_TYPE_MODULATION_WHEEL_MSB);
			if(opEvent != NULL)
				EMWinDXMIDIProducer::Instance() -> SendEvent(static_cast<EMRealtimeMIDIOutputDescriptor*>(GetTrackDestination()), 0, opEvent -> m_vDataLength, &(opEvent -> m_vBaseType), GetMIDIChannel());
			else
				; //Initiate with default value (0 ?)
*/
			break;
		}
		case EM_MESSAGE_STOP_PLAYORRECORD:
		{
			break;
		}
		default:
			break;
	};
	return true;
}
