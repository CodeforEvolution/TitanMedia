#include "NetworkCommandDeleteUser.h"
#include "EMNetworkEngine.h"
#include "EMNetworkUserHandler.h"

NetworkCommandDeleteUser::NetworkCommandDeleteUser()
{
}

NetworkCommandDeleteUser::~NetworkCommandDeleteUser()
{
}

void* NetworkCommandDeleteUser::ExecuteE(void* p_opID, void* p_opNick, void* p_opOnline)
{
	uint64  vId = *(static_cast<uint64*>(p_opID));

	m_vTaskId = EMNetworkEngine::Instance() -> GetUserHandler() -> DeleteUser(vId);

	return NULL;
}

bool NetworkCommandDeleteUser::IsUndoable()
{
	return false;
}

bool NetworkCommandDeleteUser::RequiresParameters()
{
	return true;
}