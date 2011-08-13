/*******************************************************
* Creator: Martin Johansson
* Portability: Native, very native
*-------------------------------------------------------
* 
* Martin Johansson, 01-08-01
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_NETWORK_USER_DATABASE
#define __EM_NETWORK_USER_DATABASE

#include "EMUserInfo.h"
#include "EMItemContainer.h"
#include "EMNetworkMessageQueue.h"

struct TSonorkApiUserSid;

class EMNetworkUserDatabase : public EMItemContainer<EMUserInfo>, public EMNetworkMessageQueue<EMUserInfo*>
{
public:
	EMNetworkUserDatabase();
	~EMNetworkUserDatabase();

	const EMUserInfo* GetFirstUser();
	const EMUserInfo* GetNextUser();
	uint32 GetNumberOfUsers();

	void UpdateOnlineStatus(uint64 p_oUserId, TSonorkApiUserSid* p_opSid);

	EMUserInfo* GetNextStatusChange();

	void GoOffline();
	void GoOnline();

protected:
	void ClearUsers();
	void AddUser(EMUserInfo* p_opUser);

private:
};

#endif
