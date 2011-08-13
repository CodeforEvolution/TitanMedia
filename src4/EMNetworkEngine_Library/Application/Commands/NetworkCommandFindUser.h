/*******************************************************
* Creator: Martin Johansson
* Portability: non-native
*-------------------------------------------------------
*
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __NETWORK_COMMAND_FIND_USER
#define __NETWORK_COMMAND_FIND_USER

#include "EMCommand.h"

class NetworkCommandFindUser : public EMCommand
{
public:
	NetworkCommandFindUser();
	~NetworkCommandFindUser();

	virtual void* ExecuteE(void* p_opID, void* p_opNick, void* p_opOnline);
	virtual bool IsUndoable();
	virtual bool RequiresParameters();

protected:
	int32 m_vTaskId;
};

#endif