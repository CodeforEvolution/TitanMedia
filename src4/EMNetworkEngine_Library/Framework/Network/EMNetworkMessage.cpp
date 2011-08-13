#include "EMNetworkMessage.h"
#include "EMUserInfo.h"

#include "srk_data_types.h"


EMNetworkMessage::EMNetworkMessage(const uint64 p_vId, TSonorkMsg* p_opMsg)
{
	int n = p_opMsg -> Text().String().Length();

	m_vpBody = new char[n+1];
	memcpy(m_vpBody, p_opMsg->Text().CStr(), n+1);
	m_vpBody[n] = 0;

	m_vUID = p_vId;
}

EMNetworkMessage::~EMNetworkMessage()
{
	if(m_vpBody != NULL)
		delete m_vpBody;
}

EMNetworkMessage::EMNetworkMessage()
 : m_vpBody(NULL)
{
}