#include "NetworkInfoCommandGetNextUserStatusMessage.h"
#include "EMNetworkEngine.h"
#include "EMUserInfo.h"
#include "EMNetworkUserDatabase.h"

NetworkInfoCommandGetNextUserStatusMessage::NetworkInfoCommandGetNextUserStatusMessage()
{
}

NetworkInfoCommandGetNextUserStatusMessage::~NetworkInfoCommandGetNextUserStatusMessage()
{
}

void* NetworkInfoCommandGetNextUserStatusMessage::ExecuteE(void* p_opUID, void* p_opStatus, void* p_opParameterThree)
{
	uint64* vpUID = static_cast<uint64*>(p_opUID);
	uint32* vpStatus = static_cast<uint32*>(p_opStatus);

	EMUserInfo* opUser = EMNetworkEngine::Instance() -> GetUserDatabase() -> GetNextStatusChange();

	*vpUID = opUser -> GetId();
	*vpStatus = opUser -> m_vOnline;

	int32 a = (*vpUID >>32) && 0xffffffff;
	int32 b = (*vpUID) && 0xffffffff;

	eo<<"NetworkInfoCommandGetNextUserStatusMessage: " <<a <<"."<<b <<", " <<*vpStatus <<ef;

	return NULL;
}


bool NetworkInfoCommandGetNextUserStatusMessage::IsUndoable()
{
	return false;
}

bool NetworkInfoCommandGetNextUserStatusMessage::RequiresParameters()
{
	return true;
}
