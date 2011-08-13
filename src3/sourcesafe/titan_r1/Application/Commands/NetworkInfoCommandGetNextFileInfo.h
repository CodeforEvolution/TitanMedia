/*******************************************************
* Creator: Martin Johansson
* Portability: non-native
*-------------------------------------------------------
*
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __NETWORK_INFO_COMMAND_GET_NEXT_FILE_INFO
#define __NETWORK_INFO_COMMAND_GET_NEXT_FILE_INFO

#include "EMCommand.h"

class EMNetworkMessage;

class NetworkInfoCommandGetNextFileInfo : public EMCommand
{
public:
	NetworkInfoCommandGetNextFileInfo();
	~NetworkInfoCommandGetNextFileInfo();

	virtual void* ExecuteE(void* p_opName, void*, void*);
	virtual bool IsUndoable();
	virtual bool RequiresParameters();

protected:
	uint64 m_vUID;

};

#endif