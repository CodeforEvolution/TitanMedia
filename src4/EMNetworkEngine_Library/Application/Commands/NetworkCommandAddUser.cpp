#include "NetworkCommandAddUser.h"
#include "EMNetworkEngine.h"
#include "EMNetworkUserHandler.h"

NetworkCommandAddUser::NetworkCommandAddUser()
{
}

NetworkCommandAddUser::~NetworkCommandAddUser()
{
}

void* NetworkCommandAddUser::ExecuteE(void* p_opID, void* p_opNick, void* p_opOnline)
{
	uint64  vId = *(static_cast<uint64*>(p_opID));

	m_vTaskId = EMNetworkEngine::Instance() -> GetUserHandler() -> AddUser(vId);

	return NULL;
}

bool NetworkCommandAddUser::IsUndoable()
{
	return false;
}

bool NetworkCommandAddUser::RequiresParameters()
{
	return true;
}