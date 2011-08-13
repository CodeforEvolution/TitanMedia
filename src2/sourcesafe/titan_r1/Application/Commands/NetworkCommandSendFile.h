/*******************************************************
* Creator: Martin Johansson
* Portability: non-native
*-------------------------------------------------------
*
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __NETWORK_COMMAND_SEND_FILE
#define __NETWORK_COMMAND_SEND_FILE

#include "EMCommand.h"

class NetworkCommandSendFile : public EMCommand
{
public:
	NetworkCommandSendFile();
	~NetworkCommandSendFile();

	virtual void* ExecuteE(void* p_opUIDs, void* p_opFileName, void* p_opParameterThree);
	virtual bool IsUndoable();
	virtual bool RequiresParameters();

protected:
	uint64 m_vID;
};

#endif