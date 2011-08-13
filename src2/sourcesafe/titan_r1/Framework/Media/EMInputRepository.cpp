#include "EMInputRepository.h"

#include "EMApplication.h"
#include "EMBufferRepository.h"
#include "EMMediaEngine.h"
#include "EMMediaIDManager.h"
#include "EMMediaMIDITrack.h"
#include "EMMediaProject.h"
#include "EMMediaSystemInspector.h"
#include "EMMediaTimer.h"
#include "EMMessages.h"
#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"
#include "EMRealtimeInputDescriptor.h"
#include "EMRealtimeMIDIInputDescriptor.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"

#ifdef PLATFORM_BEOS
#include "EMBeBufferRepository.h"
#endif

#ifdef PLATFORM_WINDOWS
#include "EMWinMediaSystemInspector.h"
#endif

EMInputRepository* EMInputRepository::m_opInstance = NULL;

EMInputRepository::EMInputRepository()
	:	EMMediaItemContainer<EMRealtimeInputDescriptor>(EM_TYPE_ANY),
		m_vIsInitialized(false),
		m_vID(EMMediaIDManager::MakeID())
{
	EMApplication::Instance() -> AddListener(this); //Listen for quit-messages!
	EMMediaTimer::Instance() -> AddListener(this); 
}

EMInputRepository* EMInputRepository::Instance()
{
	if(m_opInstance == NULL)
		m_opInstance = EM_new EMInputRepository();
	return m_opInstance; 
}

void EMInputRepository::Delete()
{
//	int64 v = m_opInstance -> Size();
	if(m_opInstance != NULL)
		delete m_opInstance;
	m_opInstance = NULL;
}

EMInputRepository::~EMInputRepository()
{
	EMApplication::Instance() -> RemoveListener(this); //Stop listening for quit-messages!
	EMMediaTimer::Instance() -> RemoveListener(this);
	m_vIsInitialized = false;
}

void EMInputRepository::OnItemAdd()
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

void EMInputRepository::OnItemRemove()
{
	//Same story here, as in OnItemAdd
}

bool EMInputRepository::InitCheckE()
{
	float vAudioFrameRate = 0;
	int64 vAudioBufferSize = 0;
	int64 vNum = 0;
	list<EMRealtimeInputDescriptor*>* list1 = NULL;
	list<EMRealtimeInputDescriptor*>* list2 = NULL;
	list<EMRealtimeInputDescriptor*>* list3 = NULL;

	if(! m_vIsInitialized)
	{
		LockContainer();
		vAudioFrameRate = (float) *(static_cast<int32*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_FRAMERATE)));
		vAudioBufferSize = (int64) *(static_cast<int32*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_BUFFER_SIZE)));
		vNum = static_cast<int64>(EM_AUDIO_READAHEAD * ((vAudioFrameRate) / (vAudioBufferSize / (EM_AUDIO_NUM_CHANNELS * EM_AUDIO_SAMPLESIZE))));
#ifdef PLATFORM_BEOS
		((EMBeBufferRepository*) EMBufferRepository::Instance()) -> InitializeAudioE(vAudioBufferSize, vNum); //Deleted by MediaEngine
#endif

		try
		{
			list1 = EMMediaSystemInspector::GetInstance() -> GetRealtimeAudioInputs();
			while(list1 -> size() > 0)
			{
				this -> Add(list1 -> front());
				list1 -> pop_front();
			}
			delete list1;
		}
		catch(...)
		{
//			MessageBox(NULL, "Unknown error while incorporating audio input devices.\nAttempting to continue anyway...", "Error!", MB_OK | MB_ICONWARNING);
			delete list1;
		}
		
		try
		{
			list2 = EMMediaSystemInspector::GetInstance() -> GetRealtimeVideoInputs();
			while(list2 -> size() > 0)
			{
				this -> Add(list2 -> front());
				list2 -> pop_front();
			}
			delete list2;
		}
		catch(...)
		{
//			MessageBox(NULL, "Unknown error while incorporating video input devices.\nAttempting to continue anyway...", "Error!", MB_OK | MB_ICONWARNING);
			delete list2;
		}
		
		try
		{
			list3 = EMMediaSystemInspector::GetInstance() -> GetRealtimeMIDIInputs();
			while(list3 -> size() > 0)
			{
				this -> Add(list3 -> front());
				list3 -> pop_front();
			}
			delete list3;
		}
		catch(...)
		{
//			MessageBox(NULL, "Unknown error while incorporating MIDI input devices.\nAttempting to continue anyway...", "Error!", MB_OK | MB_ICONWARNING);
			delete list3;
		}

		m_vIsInitialized = true;
		UnlockContainer();
	}
	return true;
}

bool EMInputRepository::PrepareToPlayE()
{
	return true;
}

bool EMInputRepository::StartE()
{
	LockContainer();
	try
	{
		Rewind();
		while(Current() != NULL)
		{
			Current() -> PrepareToPlayE();
			Current() -> StartE();
			Next();
		}
	}
	catch(...)
	{
	}
	UnlockContainer();
	return true;
}

bool EMInputRepository::StopE()
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

bool EMInputRepository::SetAsActiveMIDIInput(int32 p_vInputID, int32 p_vOutputID)
{
	LockContainer(); 
	try
	{
		Rewind();
		while(Current() != NULL)
		{
			EMRealtimeInputDescriptor* opInput = Current();
			if((opInput -> GetType() & EM_TYPE_MIDI) > 0)
			{
				EMRealtimeMIDIInputDescriptor* opMIDIInput = static_cast<EMRealtimeMIDIInputDescriptor*>(opInput);
				if(opMIDIInput -> GetID() == p_vInputID)
					opMIDIInput -> SetActive(true, p_vOutputID);
				else
					opMIDIInput -> SetActive(false, NULL);
			}
			Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception while setting active MIDI track!");
	}
	UnlockContainer();
	return true;
}

list<EMRecording*>* EMInputRepository::GetLastRecordings()
{
	m_oRecordings.clear();
	LockContainer();
	try
	{
		Rewind();
		while(Current() != NULL)
		{
	//		Current() -> StopE();
			if(Current() -> GetLastRecording() != NULL)
				m_oRecordings.push_back(Current() -> GetLastRecording());
			Next();
		}
	}
	catch(...)
	{
	}
	UnlockContainer();
	return &m_oRecordings;
}

int32 EMInputRepository::FindName(string p_oName)
{
	LockContainer();
	try
	{
		Rewind();
		while(Current() != NULL)
		{
			if(Current() -> GetName() == p_oName)
			{
				int32 vID = Current() -> GetID();
				UnlockContainer();
				return vID;
			}
			Next();
		}
	}
	catch(...)
	{
	}
	UnlockContainer();
	return -1;
}

bool EMInputRepository::ClearData()
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

bool EMInputRepository::SaveData(EMProjectDataSaver* p_opSaver)
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

bool EMInputRepository::LoadData(EMProjectDataLoader* p_opLoader)
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

bool EMInputRepository::StartPreviewE()
{
	LockContainer();
	try
	{
		Rewind();
		while(Current() != NULL)
		{
			Current() -> StartPreviewE();
			Next();
		}
	}
	catch(...)
	{
	}
	UnlockContainer();
	return true;
}

bool EMInputRepository::StopPreviewE()
{
	LockContainer();
	try
	{
		Rewind();
		while(Current() != NULL)
		{
			Current() -> StopPreviewE();
			Next();
		}
	}
	catch(...)
	{
	}
	UnlockContainer();
	return true;
}

bool EMInputRepository::LockContainer()
{
	return EMMediaItemContainer<EMRealtimeInputDescriptor>::LockContainer();
}

void EMInputRepository::UnlockContainer()
{
	EMMediaItemContainer<EMRealtimeInputDescriptor>::UnlockContainer();
}

bool EMInputRepository::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
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
			if(EMMediaEngine::Instance() -> GetMediaProject() -> IsArmed())
				StartE();
			return true;
		case EM_MESSAGE_STOP_PLAYORRECORD:
			if(EMMediaEngine::Instance() -> GetMediaProject() -> IsArmed())
				StopE();
			return true;
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
