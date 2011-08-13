/*******************************************************
* Creator: Martin Johansson
* Portability: non-native
*-------------------------------------------------------
*
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __NETWORK_COMMAND_ADD_USER
#define __NETWORK_COMMAND_ADD_USER

#include "EMCommand.h"

class NetworkCommandAddUser : public EMCommand
{
public:
	NetworkCommandAddUser();
	~NetworkCommandAddUser();

	virtual void* ExecuteE(void* p_opID, void*, void*);
	virtual bool IsUndoable();
	virtual bool RequiresParameters();

protected:
	int32 m_vTaskId;
};

#endif