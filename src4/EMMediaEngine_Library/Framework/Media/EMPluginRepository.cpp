#include "EMPluginRepository.h"

#include "EMApplication.h" //For being able to listen for quit-messages!
#include "EMListenerRepository.h"
#include "EMMessages.h"
#include "EMPlugin.h"

EMPluginRepository* EMPluginRepository::m_opInstance = NULL;

EMPluginRepository::EMPluginRepository()
	:	EMMediaItemContainer<EMPluginEntry>(EM_TYPE_ANY)
{
	EMApplication::Instance() -> AddListener(this); //Listen for quit-messages!
}

EMPluginRepository::~EMPluginRepository()
{
	EMApplication::Instance() -> RemoveListener(this); //Stop listening for quit-messages!
}

EMPluginRepository* EMPluginRepository::Instance()
{
	if(m_opInstance == NULL)
		m_opInstance = EM_new EMPluginRepository();
	return m_opInstance;
}

void EMPluginRepository::Delete()
{
	delete m_opInstance;
	m_opInstance = NULL;
}

void EMPluginRepository::ResetEntries()
{
	LockContainer();
	Rewind();
	while(Current() != NULL)
	{
		Current() -> Reset();
		Next();
	}
	UnlockContainer();
}

void EMPluginRepository::OnItemAdd()
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

void EMPluginRepository::OnItemRemove()
{
	//Same story here, as in OnItemAdd
}

bool EMPluginRepository::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
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