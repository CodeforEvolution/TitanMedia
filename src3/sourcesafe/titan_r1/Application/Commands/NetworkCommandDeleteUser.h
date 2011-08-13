/*******************************************************
* Creator: Martin Johansson
* Portability: non-native
*-------------------------------------------------------
*
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __NETWORK_COMMAND_DELETE_USER
#define __NETWORK_COMMAND_DELETE_USER

#include "EMCommand.h"

class NetworkCommandDeleteUser : public EMCommand
{
public:
	NetworkCommandDeleteUser();
	~NetworkCommandDeleteUser();

	virtual void* ExecuteE(void* p_opID, void*, void*);
	virtual bool IsUndoable();
	virtual bool RequiresParameters();

protected:
	int32 m_vTaskId;
};

#endif