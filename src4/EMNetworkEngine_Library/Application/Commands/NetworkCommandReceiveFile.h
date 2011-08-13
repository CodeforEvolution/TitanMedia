/*******************************************************
* Creator: Martin Johansson
* Portability: non-native
*-------------------------------------------------------
*
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __NETWORK_COMMAND_RECEIVE_FILE
#define __NETWORK_COMMAND_RECEIVE_FILE

#include "EMCommand.h"

class NetworkCommandReceiveFile : public EMCommand
{
public:
	NetworkCommandReceiveFile();
	~NetworkCommandReceiveFile();

	virtual void* ExecuteE(void* p_opID, void* p_opFileName, void* p_opParameterThree);
	virtual bool IsUndoable();
	virtual bool RequiresParameters();

protected:
	int32 m_vStatus;
};

#endif