/*******************************************************
* Creator: Martin Johansson
* Portability: non-native
*-------------------------------------------------------
*
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __NETWORK_INFO_COMMAND_GET_NEXT_MESSAGE
#define __NETWORK_INFO_COMMAND_GET_NEXT_MESSAGE

#include "EMCommand.h"

class EMNetworkMessage;

class NetworkInfoCommandGetNextMessage : public EMCommand
{
public:
	NetworkInfoCommandGetNextMessage();
	~NetworkInfoCommandGetNextMessage();

	virtual void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
	virtual bool IsUndoable();
	virtual bool RequiresParameters();

protected:
	EMNetworkMessage* m_opMsg;

};

#endif