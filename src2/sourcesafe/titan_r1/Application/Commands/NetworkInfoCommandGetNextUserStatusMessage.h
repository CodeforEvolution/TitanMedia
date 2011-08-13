/*******************************************************
* Creator: Martin Johansson
* Portability: non-native
*-------------------------------------------------------
*
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __NETWORK_INFO_COMMAND_GET_NEXT_USER_STATUS_MESSAGE
#define __NETWORK_INFO_COMMAND_GET_NEXT_USER_STATUS_MESSAGE

#include "EMCommand.h"

class NetworkInfoCommandGetNextUserStatusMessage : public EMCommand
{
public:
	NetworkInfoCommandGetNextUserStatusMessage();
	~NetworkInfoCommandGetNextUserStatusMessage();

	virtual void* ExecuteE(void* p_opUID, void* p_opStatus, void* p_opParameterThree);
	virtual bool IsUndoable();
	virtual bool RequiresParameters();

protected:
};

#endif