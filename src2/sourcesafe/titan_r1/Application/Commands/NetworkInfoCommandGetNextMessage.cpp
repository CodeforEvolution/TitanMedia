#include "NetworkInfoCommandGetNextMessage.h"

#include "EMNetworkEngine.h"
#include "EMNetworkMessage.h"


NetworkInfoCommandGetNextMessage::NetworkInfoCommandGetNextMessage()
  : m_opMsg(NULL)
{
}

NetworkInfoCommandGetNextMessage::~NetworkInfoCommandGetNextMessage()
{
	if(m_opMsg != NULL)
		delete m_opMsg;
}

void* NetworkInfoCommandGetNextMessage::ExecuteE(void* p_opBody, void* p_opParameterTwo, void* p_opParameterThree)
{
	char* vpBody = static_cast<char*>(p_opBody);

	m_opMsg = EMNetworkEngine::Instance() -> GetMessageQueue() -> GetMessage();
	strcpy(vpBody, m_opMsg -> m_vpBody);

	int32 a = (m_opMsg -> m_vUID >>32) && 0xffffffff;
	int32 b = (m_opMsg -> m_vUID) && 0xffffffff;

	eo<<"NetworkInfoCommandGetNextMessage: " <<a <<"." <<b  <<", " <<vpBody <<ef;
 
	return &(m_opMsg -> m_vUID);
}

bool NetworkInfoCommandGetNextMessage::IsUndoable()
{
	return false;
}

bool NetworkInfoCommandGetNextMessage::RequiresParameters()
{
	return true;
}