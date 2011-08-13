/*******************************************************
* Creator: Martin Johansson
* Portability: non-native
*-------------------------------------------------------
*
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __NETWORK_INFO_COMMAND_GET_NEXT_FOUND_USER
#define __NETWORK_INFO_COMMAND_GET_NEXT_FOUND_USER

#include "EMCommand.h"

class EMUserInfo;

class NetworkInfoCommandGetNextFoundUser : public EMCommand
{
public:
	NetworkInfoCommandGetNextFoundUser();
	~NetworkInfoCommandGetNextFoundUser();

	virtual void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
	virtual bool IsUndoable();
	virtual bool RequiresParameters();

protected:
	EMUserInfo* m_opUser;

};

#endif