#include "NetworkCommandSendMessage.h"
#include "EMNetworkEngine.h"
#include "EMNetworkMessageSender.h"

NetworkCommandSendMessage::NetworkCommandSendMessage()
{
}

NetworkCommandSendMessage::~NetworkCommandSendMessage()
{
}

void* NetworkCommandSendMessage::ExecuteE(void* p_opUIDs, void* p_opMsgBody, void* p_opParameterThree)
{
	uint64* vpUIDs = static_cast<uint64*>(p_opUIDs);
	char*   vpBody = static_cast<char*>(p_opMsgBody);

	eo << "NetworkCommandSendMessage - Pre" << ef;


	EMNetworkEngine::Instance() -> GetMessageSender() -> SendTextMessage(vpUIDs, vpBody);

	eo << "NetworkCommandSendMessage - Post" << ef;
	return NULL;
}

bool NetworkCommandSendMessage::IsUndoable()
{
	return false;
}

bool NetworkCommandSendMessage::RequiresParameters()
{
	return true;
}