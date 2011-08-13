#ifndef __EM_ENCODER_REPOSITORY_CPP
#define __EM_ENCODER_REPOSITORY_CPP

#include "EMEncoderRepository.h"

#include "EMApplication.h" //For being able to listen for quit-messages!
#include "EMListenerRepository.h"
#include "EMMediaSystemInspector.h"
#include "EMMessages.h"

EMEncoderRepository* EMEncoderRepository::m_opInstance = NULL;

EMEncoderRepository::EMEncoderRepository()
	:	EMMediaItemContainer<EMRealtimeEncoderDescriptor>(EM_TYPE_ANY),
		m_vIsInitialized(false),
		m_vID(EMMediaIDManager::MakeID())
{
	EMApplication::Instance() -> AddListener(this); //Listen for quit-messages!
}

EMEncoderRepository* EMEncoderRepository::Instance()
{
	if(m_opInstance == NULL)
		m_opInstance = new EMEncoderRepository();
	return m_opInstance;
}

void EMEncoderRepository::Delete()
{
	delete m_opInstance;
	m_opInstance = NULL;
}

EMEncoderRepository::~EMEncoderRepository()
{
	EMApplication::Instance() -> RemoveListener(this); //Stop listening for quit-messages!
	m_vIsInitialized = false;
}

void EMEncoderRepository::OnItemAdd()
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

void EMEncoderRepository::OnItemRemove()
{
	//Same story here, as in OnItemAdd
}

bool EMEncoderRepository::InitCheckE()
{
	float vAudioFrameRate = 0;
	int64 vNum = 0;
	list<EMRealtimeEncoderDescriptor*>* list1 = NULL;
	list<EMRealtimeEncoderDescriptor*>* list2 = NULL;

	if(! m_vIsInitialized)
	{

		try
		{
			list1 = EMMediaSystemInspector::GetInstance() -> GetRealtimeAudioEncoders();
	
			while(list1 -> size() > 0)
			{
				this -> Add(list1 -> front());
				list1 -> pop_front();
			}
			delete list1;
		
			m_vIsInitialized = true;
			list2 = EMMediaSystemInspector::GetInstance() -> GetRealtimeVideoEncoders();

			while(list2 -> size() > 0)
			{
				this -> Add(list2 -> front());
				list2 -> pop_front();
			}
			delete list2;

		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in EMEncoderRepository::InitCheckE()");
		}

	}
	return true;
}

bool EMEncoderRepository::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
		case EM_QUIT_PHASE_ONE:
			return false;
		case EM_QUIT_PHASE_TWO:
			break;
		case EM_QUIT_PHASE_THREE:
			break;
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

#endif