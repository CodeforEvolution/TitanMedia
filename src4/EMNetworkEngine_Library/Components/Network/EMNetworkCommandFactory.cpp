#include "EMNetworkCommandFactory.h"
#include "EMNetworkEngine.h"

#include "NetworkCommandSendMessage.h"
#include "NetworkCommandSendFile.h"
#include "NetworkCommandReceiveFile.h"
#include "NetworkCommandFindUser.h"
#include "NetworkCommandAddUser.h"
#include "NetworkCommandDeleteUser.h"

#include "NetworkInfoCommandGetNextMessage.h"
#include "NetworkInfoCommandGetNextUserStatusMessage.h"
#include "NetworkInfoCommandGetNumberOfUsers.h"
#include "NetworkInfoCommandGetUserInfos.h"
#include "NetworkInfoCommandGetNextFileInfo.h"
#include "NetworkInfoCommandGetNextDownloadedFile.h"
#include "NetworkInfoCommandGetNextFoundUser.h"

#include "CommandIDs.h"

extern "C" __declspec(dllexport) EMNetworkCommandFactory* InstanceNetworkCommandFactory() 
{ 
	return EMNetworkEngine::Instance() -> GetCommandFactory(); 
}

extern "C" __declspec(dllexport) void DeleteNetworkCommandFactory() {
	EMNetworkCommandFactory::Delete(); 
}

EMNetworkCommandFactory* EMNetworkCommandFactory::m_opInstance = NULL;

EMNetworkCommandFactory* EMNetworkCommandFactory::Instance()
{
	if(m_opInstance == NULL)
		m_opInstance = EM_new EMNetworkCommandFactory();
	return m_opInstance;
}

void EMNetworkCommandFactory::Delete()
{
	delete m_opInstance;
	m_opInstance = NULL;
}

EMNetworkCommandFactory::EMNetworkCommandFactory()
{
}

EMNetworkCommandFactory::~EMNetworkCommandFactory()
{
}

EMCommand* EMNetworkCommandFactory::CreateNetworkCommand(uint32 p_vType)
{
	switch(p_vType)
	{
		case NETWORK_COMMAND_SEND_MESSAGE:
			return EM_new NetworkCommandSendMessage();
		case NETWORK_COMMAND_SEND_FILE:
			return EM_new NetworkCommandSendFile();
		case NETWORK_COMMAND_RECEIVE_FILE:
			return EM_new NetworkCommandReceiveFile();
		case NETWORK_COMMAND_FIND_USER:
			return EM_new NetworkCommandFindUser();
		case NETWORK_COMMAND_ADD_USER:
			return EM_new NetworkCommandAddUser();
		case NETWORK_COMMAND_DELETE_USER:
			return EM_new NetworkCommandDeleteUser();


		case NETWORK_INFO_COMMAND_GET_MESSAGE:
			return EM_new NetworkInfoCommandGetNextMessage();
		case NETWORK_INFO_COMMAND_USER_STATUS_MESSAGE:
			return EM_new NetworkInfoCommandGetNextUserStatusMessage();
		case NETWORK_INFO_COMMAND_NUMBER_OF_USERS:
			return EM_new NetworkInfoCommandGetNumberOfUsers();
		case NETWORK_INFO_COMMAND_GET_USER_INFOS:
			return EM_new NetworkInfoCommandGetUserInfos();
		case NETWORK_INFO_COMMAND_GET_NEXT_FILE_INFO:
			return EM_new NetworkInfoCommandGetNextFileInfo();
		case NETWORK_INFO_COMMAND_GET_NEXT_DOWNLOADED_FILE:
			return EM_new NetworkInfoCommandGetNextDownloadedFile();
		case NETWORK_INFO_COMMAND_GET_NEXT_FOUND_USER:
			return EM_new NetworkInfoCommandGetNextFoundUser();


		default:
			EMDebugger("ERROR! EMNetworkCommandFactory could not find that network command!");
	}
	
	return NULL;
}
