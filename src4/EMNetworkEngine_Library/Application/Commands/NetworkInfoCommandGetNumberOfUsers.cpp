#include "NetworkInfoCommandGetNumberOfUsers.h"
#include "EMNetworkEngine.h"
#include "EMNetworkUserDatabase.h"


NetworkInfoCommandGetNumberOfUsers::NetworkInfoCommandGetNumberOfUsers()
  : m_vNum(0)
{
}

NetworkInfoCommandGetNumberOfUsers::~NetworkInfoCommandGetNumberOfUsers()
{
}

void* NetworkInfoCommandGetNumberOfUsers::ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree)
{
	if(EMNetworkEngine::Instance() -> IsConnected())
	{
		m_vNum = EMNetworkEngine::Instance() -> GetUserDatabase() -> GetNumberOfUsers();
	}
	else
	{
		m_vNum = 0;
	}

	return &m_vNum;
}

bool NetworkInfoCommandGetNumberOfUsers::IsUndoable()
{
	return false;
}

bool NetworkInfoCommandGetNumberOfUsers::RequiresParameters()
{
	return true;
}
