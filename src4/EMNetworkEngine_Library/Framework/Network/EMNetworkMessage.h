/*******************************************************
* Creator: Martin Johansson
* Portability: Non-Native
*-------------------------------------------------------
* 
* Martin Johansson, 01-08-02
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_NETWORK_MESSAGE
#define __EM_NETWORK_MESSAGE

struct TSonorkMsgHeader;
struct TSonorkMsg;

class EMNetworkMessage
{
public:
	EMNetworkMessage(const uint64 p_vId, TSonorkMsg* p_opMsg);
	~EMNetworkMessage();

	uint64 m_vUID;
	char*  m_vpBody;

protected:
	EMNetworkMessage();
private:
};

#endif
