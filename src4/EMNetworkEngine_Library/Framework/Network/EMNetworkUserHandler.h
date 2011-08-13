/*******************************************************
* Creator: Martin Johansson
* Portability: Native, very native
*-------------------------------------------------------
* 
* Handles searching/adding/deleting users. 
*
* Martin Johansson, 01-08-01
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_NETWORK_USER_HANDLER
#define __EM_NETWORK_USER_HANDLER

#include "EMUserInfo.h"
#include "EMItemContainer.h"
#include "EMNetworkSearchItem.h"
#include "EMNetworkMessageQueue.h"

class EMNetworkUserHandler : public EMItemContainer<EMNetworkSearchItem> , public EMNetworkMessageQueue<EMUserInfo*>
{
public:
	EMNetworkUserHandler();
	~EMNetworkUserHandler();

	uint64 AddUser(uint64 p_vID);
	uint64 DeleteUser(uint64 p_vID);
	uint64 FindUser(uint64 p_vID, const char* p_vNick, bool p_vOnlineOnly);
	EMUserInfo* GetNextFound(uint64 p_vTask = 0);

	void AddFoundUser(EMUserInfo* p_opUser);
	void NotifySearchEnded(int64 p_vTaskId);
	void NotifyUserAdded(int64 p_vTaskId);
	void NotifyUserDeleted(int64 p_vTaskId);

	void TimeSlot();

protected:
private:
};

#endif
