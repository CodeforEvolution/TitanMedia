/*******************************************************
* Creator: Martin Johansson
* Portability: non-native
*-------------------------------------------------------
*
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __NETWORK_INFO_COMMAND_GET_NEXT_DOWNLOADED_FILE
#define __NETWORK_INFO_COMMAND_GET_NEXT_DOWNLOADED_FILE

#include "EMCommand.h"

class EMNetworkMessage;

class NetworkInfoCommandGetNextDownloadedFile : public EMCommand
{
public:
	NetworkInfoCommandGetNextDownloadedFile();
	~NetworkInfoCommandGetNextDownloadedFile();

	virtual void* ExecuteE(void* p_opName, void*, void*);
	virtual bool IsUndoable();
	virtual bool RequiresParameters();

protected:
	char* m_vpName;
	uint64 m_vUID;
};

#endif