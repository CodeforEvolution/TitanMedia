#include "NetworkInfoCommandGetNextFoundUser.h"
#include "EMNetworkEngine.h"
#include "EMUserInfo.h"
#include "EMNetworkUserHandler.h"


NetworkInfoCommandGetNextFoundUser::NetworkInfoCommandGetNextFoundUser()
  :	m_opUser(NULL)
{
}

NetworkInfoCommandGetNextFoundUser::~NetworkInfoCommandGetNextFoundUser()
{
	if(m_opUser != NULL)
		delete m_opUser;
}

void* NetworkInfoCommandGetNextFoundUser::ExecuteE(void* p_vp1, void*, void*)
{
	char* opNick = static_cast<char*>(p_vp1);

	m_opUser = EMNetworkEngine::Instance() -> GetUserHandler() -> GetMessage();

	strncpy(opNick, m_opUser -> m_opName.c_str(), 75); 

	return &m_opUser -> m_opUID;
}

bool NetworkInfoCommandGetNextFoundUser::IsUndoable()
{
	return false;
}

bool NetworkInfoCommandGetNextFoundUser::RequiresParameters()
{
	return true;
}
