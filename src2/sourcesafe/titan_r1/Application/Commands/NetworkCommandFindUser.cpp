#include "NetworkCommandFindUser.h"
#include "EMNetworkEngine.h"
#include "EMNetworkUserHandler.h"

NetworkCommandFindUser::NetworkCommandFindUser()
{
}

NetworkCommandFindUser::~NetworkCommandFindUser()
{
}

void* NetworkCommandFindUser::ExecuteE(void* p_opID, void* p_opNick, void* p_opOnline)
{
	char* vTmp = "";

	uint64  vId = p_opID == NULL ? 0 : *(static_cast<uint64*>(p_opID));
	char* vpNick = p_opNick == NULL ? vTmp : static_cast<char*>(p_opNick);
	bool vOnline = *(static_cast<bool*>(p_opOnline));

	eo << "NetworkCommandFindUser::ExecuteE" << ef;

	m_vTaskId = EMNetworkEngine::Instance() -> GetUserHandler() -> FindUser(vId, vpNick, vOnline);

	return &m_vTaskId;
}

bool NetworkCommandFindUser::IsUndoable()
{
	return false;
}

bool NetworkCommandFindUser::RequiresParameters()
{
	return true;
}