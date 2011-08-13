#include "EMOutputRepository.h"

#include "EMApplication.h" //For being able to listen for quit-messages!
#include "EMBeBufferRepository.h"
#include "EMBufferRepository.h"
#include "EMMediaEffectTrackRepository.h"
#include "EMMediaEngine.h"
#include "EMMediaGlobals.h"
#include "EMMediaIDManager.h"
#include "EMMediaProject.h"
#include "EMMediaSystemInspector.h"
#include "EMMediaTrackRepository.h"
#include "EMMediaUtility.h"
#include "EMMessages.h"
#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"
#include "EMRealtimeMIDIOutputDescriptor.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"

EMOutputRepository* EMOutputRepository::m_opInstance = NULL;

EMOutputRepository::EMOutputRepository() 
	:	EMMediaItemContainer<EMRealtimeOutputDescriptor>(EM_TYPE_ANY),
		m_vIsInitialized(false)
{
	EMApplication::Instance() -> AddListener(this); //Listen for quit-messages!
	m_vID = EMMediaIDManager::MakeID();
}

EMOutputRepository* EMOutputRepository::Instance()
{
	if(m_opInstance == NULL)
		m_opInstance = EM_new EMOutputRepository();
	return m_opInstance;
}

void EMOutputRepository::Delete() //Deleted by MediaEngine
{
//	int64 v = m_opInstance -> Size(); 
	if(m_opInstance != NULL)
		delete m_opInstance;
	m_opInstance = NULL;
}

EMOutputRepository::~EMOutputRepository()
{
	EMApplication::Instance() -> RemoveListener(this); //Stop listening for quit-messages!
	//StopE(); //Moved to MessageReceived EM_QUIT_PHASE_THREE
}

void EMOutputRepository::OnItemAdd()
{
	//We could make the project dirty here, but we won't!
	//Why, you ask you simple person? Because the encoder repository
	//isn't really part of the project, and is not changed when
	//you load/save a project. So encoders can't be added when a
	//project is already active, and therefor, making the project
	//dirty here is very strange! 
	//
	//Added by Richard, 010821
}

void EMOutputRepository::OnItemRemove()
{
	//Same story here, as in OnItemAdd
}

int32 EMOutputRepository::GetDefaultOutput(EMMediaType p_eType)
{
	LockContainer();
	try
	{
		if(Size() > 0)
		{
			Rewind();
			while(Current() != NULL)
			{
				if((Current() -> GetType() & p_eType) > 0)
				{
					int32 vID = Current() -> GetID();
					UnlockContainer();
					return vID;
				}
				Next();
			}
		}
	}
	catch(...)
	{
	}
	UnlockContainer();
	return -1;
}

EMRealtimeOutputDescriptor* EMOutputRepository::SearchForName(string p_oName)
{
	LockContainer();
	try
	{
		Rewind();
		while(Current() != NULL)
		{
			if(Current() -> GetName() == p_oName)
			{
				EMRealtimeOutputDescriptor* opReturn = Current();
				UnlockContainer();
				return opReturn;
			}
			Next();
		}
	}
	catch(...)
	{
	}
	UnlockContainer();
	return NULL;
}

bool EMOutputRepository::InitCheckE()
{
	if(m_vIsInitialized)
		return true;



	float vAudioFrameRate = (float) *static_cast<int32*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_FRAMERATE));
	int64 vAudioBufferSize = (int64) *(static_cast<int32*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_BUFFER_SIZE)));
	int64 vNum = static_cast<int64>(EM_AUDIO_READAHEAD * ((vAudioFrameRate) / (vAudioBufferSize / (EM_AUDIO_NUM_CHANNELS * EM_AUDIO_SAMPLESIZE))));

#ifdef PLATFORM_BEOS
	((EMBeBufferRepository*) EMBufferRepository::Instance()) -> InitializeAudioE(vAudioBufferSize, vNum); //Deleted by MediaEngine
#endif
#ifdef PLATFORM_WINDOWS
#endif

	//Get audio outputs
	list<EMRealtimeOutputDescriptor*>* list1 = EMMediaSystemInspector::GetInstance() -> GetRealtimeAudioOutputs();
	if(list1 == NULL)
	{
		EMDebugger("ERROR! No audio outputs found!");
		m_vIsInitialized = false;
		return false;
	}

	LockContainer();
	try
	{
		while(list1 -> size() > 0)
		{
			EMRealtimeOutputDescriptor* opDesc = list1 -> front();
			list1 -> pop_front();
			if(opDesc -> IsInitialized())
			{
				this -> Add(opDesc);
			}
			else
				delete opDesc;
		}
	}
	catch(...)
	{
	}

	//Get video outputs
	list<EMRealtimeOutputDescriptor*>* list2 = EMMediaSystemInspector::GetInstance() -> GetRealtimeVideoOutputs();
	if(list2 == NULL)
	{
		m_vIsInitialized = false;
		UnlockContainer();
		return false;
	}
	try
	{
		while(list2 -> size() > 0)
		{
			EMRealtimeOutputDescriptor* opDesc = list2 -> front();
			list2 -> pop_front();
			if(opDesc -> IsInitialized())
			{
				this -> Add(opDesc);
			}
			else
				delete opDesc;
		} 
	}
	catch(...)
	{
	}

	//Get MIDI outputs
	list<EMRealtimeOutputDescriptor*>* list3 = EMMediaSystemInspector::GetInstance() -> GetRealtimeMIDIOutputs();
	if(list3 == NULL)
	{
		EMDebugger("ERROR! No MIDI outputs found!");
		m_vIsInitialized = false;
		UnlockContainer();
		return false;
	}
	try
	{
		while(list3 -> size() > 0)
		{
			EMRealtimeOutputDescriptor* opDesc = list3 -> front();
			list3 -> pop_front();
			if(opDesc -> IsInitialized())
			{
				this -> Add(opDesc);
			}
			else
				delete opDesc;
		} 
	}
	catch(...)
	{
	}

	UnlockContainer();

	delete list1;
	delete list2;
	delete list3;

	m_vIsInitialized = true;

	if(! PrepareToPlayE())
		return false;
	
	if(! StartE())
		return false;

	return true;
}

bool EMOutputRepository::PrepareToPlayE()
{
	LockContainer();
	try
	{
		Rewind();
		while(Current() != NULL) 
		{
			if(! Current() -> PrepareToPlayE())
			{
				UnlockContainer();
				return false;
			}
			Next();
		}
	}
	catch(...)
	{
	}
	UnlockContainer();
	return true;
}

bool EMOutputRepository::StartE()
{
	LockContainer();
	try
	{
		Rewind();
		while(Current() != NULL)
		{	if(!Current() -> IsRenderOutput())
				Current() -> StartE();
			Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! OutputRepository caught an exception while starting output!");
	}
	UnlockContainer();
	return true;
}

bool EMOutputRepository::StopE()
{
	LockContainer();
	try
	{
		Rewind();
		while(Current() != NULL)
		{
			Current() -> StopE();
			Next();
		}
	}
	catch(...)
	{
	}
	UnlockContainer();
	return true;
}

bool EMOutputRepository::SetAsActiveMIDIOutput(int32 p_vID)
{
	LockContainer();
	try
	{
		Rewind();
		while(Current() != NULL)
		{
			EMRealtimeOutputDescriptor* opOutput = Current();
			if((opOutput -> GetType() & EM_TYPE_MIDI) > 0)
			{
				EMRealtimeMIDIOutputDescriptor* opMIDIOutput = static_cast<EMRealtimeMIDIOutputDescriptor*>(opOutput);
				if(opMIDIOutput -> GetID() == p_vID)
					opMIDIOutput -> SetActive(true);
				else
					opMIDIOutput -> SetActive(false);
			}
			Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception while setting active MIDI output!");
	}
	UnlockContainer();
	return true;
}

bool EMOutputRepository::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
		case EM_MESSAGE_AUDIO_TRACK_ACTIVATED:
			break;
		case EM_MESSAGE_VIDEO_TRACK_ACTIVATED:
			break;
		case EM_MESSAGE_AUDIO_TRACK_DEACTIVATED:
			break;
		case EM_MESSAGE_VIDEO_TRACK_DEACTIVATED:
			break;
//		case EM_MESSAGE_MIDI_RECORDING_STARTED:
//			break;
//		case EM_MESSAGE_MIDI_RECORDING_STOPPED:
//			break;
		case EM_MESSAGE_BEGIN_PLAYORRECORD:
			break;
		case EM_MESSAGE_STOP_PLAYORRECORD:
			break;
		case EM_QUIT_PHASE_ONE:
			return false;
		case EM_QUIT_PHASE_TWO:
			break;
		case EM_QUIT_PHASE_THREE:
		{
			StopE();
			return true;
		}
		case EM_QUIT_PHASE_FOUR:
			break;
		case EM_QUIT_PHASE_FIVE:
			break;
		case EM_QUIT_PHASE_SIX:
			break;
		case EM_QUIT_ABORTED:
			break;
		default:
			break;
	};
	return false;
}

EMRealtimeOutputDescriptor* EMOutputRepository::FindClockMaster()
{
	EMMediaProject* opProject = EMMediaEngine::Instance() -> GetMediaProject();
	EMMediaTrackRepository* opUsedTracks = opProject -> GetUsedTracks();
	EMMediaTrackRepository* opUnusedTracks = opProject -> GetUnusedTracks();
//	EMMediaTrack* opTrack = NULL;

	EMRealtimeOutputDescriptor* opAudioOutput = NULL;
	EMRealtimeOutputDescriptor* opVideoOutput = NULL;
	EMRealtimeOutputDescriptor* opMIDIOutput = NULL;

	opUsedTracks -> LockContainer();
	try
	{
		opUsedTracks -> Rewind();
		while(opUsedTracks -> Current() != NULL)
		{
			//Mastering output selection scheme:
			//	-	If there is an audio track with clips in it, chose the first track's output as clock master
			//	-	If there are no active audio tracks, chose the first active video track's output as clock master
			//	-	If there are no active video tracks, chose the first unused track's output as clock master
			EMBufferDestination* opDest = opUsedTracks -> Current() -> GetTrackDestination();
//			_ASSERT(opDest != NULL);
			int32 vID = opDest -> GetID();
			
			
			//If it's an audio track it always has an effect track. The FX track then has the physical output instead of the media track itself!
			if((opUsedTracks -> Current() -> GetType() & EM_TYPE_ANY_AUDIO) > 0)
			{
				if(EMMediaEffectTrackRepository::Instance() -> Find(vID) != NULL)
				{
					if(EMMediaEffectTrackRepository::Instance() -> Find(vID) -> GetOutput() != NULL)
						vID = EMMediaEffectTrackRepository::Instance() -> Find(vID) -> GetOutput() -> GetID();
					else
						vID = -1;
				}
				else
					vID = -1;
			}

			
			if((opUsedTracks -> Current() -> GetType() & EM_TYPE_ANY_VIDEO) > 0)
			{
				while(opDest -> GetPosition() != EM_OUTPUT && opDest -> GetDestination() != NULL)
					opDest = opDest -> GetDestination();

				vID = opDest -> GetID();
			}

			EMRealtimeOutputDescriptor* opOutput = Find(vID);

			if(opOutput != NULL)
			{
				if((opOutput -> GetType() & EM_TYPE_ANY_AUDIO) > 0)
				{
					if(opAudioOutput == NULL)
						opAudioOutput = opOutput;
					else
						opOutput -> SetClockMaster(false);
				}
				else if((opOutput -> GetType() & EM_TYPE_ANY_VIDEO) > 0)
				{
					if(opVideoOutput == NULL)
						opVideoOutput = opOutput;
					else
						opOutput -> SetClockMaster(false);
				}
				else if((opOutput -> GetType() & EM_TYPE_MIDI) > 0)
				{
					if(opMIDIOutput == NULL)
						opMIDIOutput = opOutput;
					else
						opOutput -> SetClockMaster(false);
				}
			}

			opUsedTracks -> Next();
		}
	}
	catch(...)
	{
	}
	opUsedTracks -> UnlockContainer();
	
	if(opAudioOutput != NULL)
	{
		if(opVideoOutput != NULL)
			opVideoOutput -> SetClockMaster(false);
		if(opMIDIOutput != NULL)
			opMIDIOutput -> SetClockMaster(false);
		return opAudioOutput;
	}
	else if(opMIDIOutput != NULL)
	{
		if(opVideoOutput != NULL)
			opVideoOutput -> SetClockMaster(false);
		return opMIDIOutput;
	}
	else if(opVideoOutput != NULL)
	{
		return opVideoOutput;
	}
	
//	MessageBox(NULL, "DEBUG WARNING! Didn't find any output suitable for mastering the clock. Choosing first track's output instead!", "Warning!", MB_OK);
	opUnusedTracks -> LockContainer();
	opUnusedTracks -> Rewind();

	if(opUnusedTracks -> Current() == NULL)
	{
		opUnusedTracks -> UnlockContainer();
		return NULL;
	}
	if(opUnusedTracks -> Current() -> GetTrackDestination() == NULL)
	{
		opUnusedTracks -> UnlockContainer();
		return NULL;
	}

	int32 vID = opUnusedTracks -> Current() -> GetTrackDestination() -> GetID();

	//If it's an audio track it always has an effect track. The FX track then has the physical output instead of the media track itself!
	if((opUnusedTracks -> Current() -> GetType() & EM_TYPE_ANY_AUDIO) > 0)
	{
		opUnusedTracks -> UnlockContainer();

		EMMediaEffectTrackRepository::Instance() -> LockContainer();
		if(EMMediaEffectTrackRepository::Instance() -> Find(vID) != NULL)
		{
			if(EMMediaEffectTrackRepository::Instance() -> Find(vID) -> GetOutput() != NULL)
				vID = EMMediaEffectTrackRepository::Instance() -> Find(vID) -> GetOutput() -> GetID();
			else
				vID = -1;
		}
		else
			vID = -1;
		EMMediaEffectTrackRepository::Instance() -> UnlockContainer();
	}
	else
		opUnusedTracks -> UnlockContainer();

	LockContainer();
	EMRealtimeOutputDescriptor* opOutput = Find(vID);
	UnlockContainer();
	return opOutput;
}

bool EMOutputRepository::ClearData()
{
	m_vIsInitialized = false;
	LockContainer();
	try
	{
		Rewind();
		while(Current() != NULL)
		{
			Current() -> ClearData();
			Next();
		}
	}
	catch(...)
	{
	}
	UnlockContainer();
	return true;
}

bool EMOutputRepository::SaveData(EMProjectDataSaver* p_opSaver)
{
	LockContainer();
	try
	{
		Rewind();
		while(Current() != NULL)
		{
			Current() -> SaveData(p_opSaver);
			Next();
		}
	}
	catch(...)
	{
	}
	UnlockContainer();
	return true;
}

bool EMOutputRepository::LoadData(EMProjectDataLoader* p_opLoader)
{
	LockContainer();
	try
	{
		Rewind();
		while(Current() != NULL)
		{
			Current() -> LoadData(p_opLoader);
			Next();
		}
	}
	catch(...)
	{
	}
	UnlockContainer();
	return true;
}
