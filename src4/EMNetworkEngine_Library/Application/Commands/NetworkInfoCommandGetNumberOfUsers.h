/*******************************************************
* Creator: Martin Johansson
* Portability: non-native
*-------------------------------------------------------
*
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __NETWORK_INFO_COMMAND_GET_NUMBER_OF_USERS
#define __NETWORK_INFO_COMMAND_GET_NUMBER_OF_USERS

#include "EMCommand.h"

class NetworkInfoCommandGetNumberOfUsers : public EMCommand
{
public:
	NetworkInfoCommandGetNumberOfUsers();
	~NetworkInfoCommandGetNumberOfUsers();

	virtual void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
	virtual bool IsUndoable();
	virtual bool RequiresParameters();

protected:
	int32 m_vNum;
};

#endif