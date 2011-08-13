/*******************************************************
* Creator: Martin Johansson
* Portability: Native, very native
*-------------------------------------------------------
* 
* Martin Johansson, 01-08-01
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_NETWORK_MESSAGE_SENDER
#define __EM_NETWORK_MESSAGE_SENDER

#include "EMNetworkSendFileInfo.h"
#include "EMItemContainer.h"

struct TSonorkApiTaskEvent;
struct TSonorkApiFileSession;

class EMNetworkMessageSender : public EMItemContainer<EMNetworkSendFileInfo>
{
public:
	EMNetworkMessageSender();
	~EMNetworkMessageSender();

	void SendTextMessage(uint64* p_opReceivers, char* p_opText);
	uint64 SendFileMessage(uint64 p_vReceiver, const char* p_opFile);

	void TimeSlot();
protected:
private:
};

#endif
