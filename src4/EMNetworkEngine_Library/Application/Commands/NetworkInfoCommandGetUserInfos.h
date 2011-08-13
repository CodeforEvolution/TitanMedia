/*******************************************************
* Creator: Martin Johansson
* Portability: non-native
*-------------------------------------------------------
*
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __NETWORK_INFO_COMMAND_GET_USER_INFOS
#define __NETWORK_INFO_COMMAND_GET_USER_INFOS

#include "EMCommand.h"

class NetworkInfoCommandGetUserInfos : public EMCommand
{
public:
	NetworkInfoCommandGetUserInfos();
	~NetworkInfoCommandGetUserInfos();

	virtual void* ExecuteE(void* p_opUIDs, void* p_opNames, void* p_opStatus);
	virtual bool IsUndoable();
	virtual bool RequiresParameters();

protected:
	int32 m_vNum;
};

#endif