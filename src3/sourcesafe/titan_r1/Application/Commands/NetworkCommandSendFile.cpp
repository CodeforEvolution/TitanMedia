#include "NetworkCommandSendFile.h"
#include "EMNetworkEngine.h"
#include "EMNetworkMessageSender.h"

NetworkCommandSendFile::NetworkCommandSendFile()
{
}

NetworkCommandSendFile::~NetworkCommandSendFile()
{
}

void* NetworkCommandSendFile::ExecuteE(void* p_opUIDs, void* p_opFileName, void* p_opParameterThree)
{
	uint64  vUID = *(static_cast<uint64*>(p_opUIDs));
	char*   vpName = static_cast<char*>(p_opFileName);

	eo << "NetworkCommandSendFile::ExecuteE - Sending file: " << vpName <<  ef;

	m_vID = EMNetworkEngine::Instance() -> GetMessageSender() -> SendFileMessage(vUID, vpName);

	return &m_vID;
}

bool NetworkCommandSendFile::IsUndoable()
{
	return false;
}

bool NetworkCommandSendFile::RequiresParameters()
{
	return true;
}