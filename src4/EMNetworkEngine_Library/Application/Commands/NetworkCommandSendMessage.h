/*******************************************************
* Creator: Martin Johansson
* Portability: non-native
*-------------------------------------------------------
*
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __NETWORK_COMMAND_SEND_MESSAGE
#define __NETWORK_COMMAND_SEND_MESSAGE

#include "EMCommand.h"

class NetworkCommandSendMessage : public EMCommand
{
public:
	NetworkCommandSendMessage();
	~NetworkCommandSendMessage();

	virtual void* ExecuteE(void* p_opUIDs, void* p_opMsgBody, void* p_opParameterThree);
	virtual bool IsUndoable();
	virtual bool RequiresParameters();

protected:
};

#endif