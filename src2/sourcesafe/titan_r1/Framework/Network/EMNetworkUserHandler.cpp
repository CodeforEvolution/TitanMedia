#include "EMNetworkUserHandler.h"
#include "EMNetworkEngine.h"

#include "EMMessages.h"

#include "sonorkvcapi.h"

EMNetworkUserHandler::EMNetworkUserHandler()
{
}

EMNetworkUserHandler::~EMNetworkUserHandler()
{
	LockContainer();
	Rewind();
	while(Current() != NULL)
	{
		EMNetworkSearchItem* opTmp = Current();
		Remove(opTmp);
		delete opTmp;
		Rewind();
	}

	UnlockContainer();
}

uint64 EMNetworkUserHandler::AddUser(uint64 p_vID)
{
	TSonorkId vID;

	vID.Set(EMUserInfo::ConvertId(p_vID));

	DWORD vTaskId = EMNetworkEngine::Instance() -> GetAPI() -> AddUserAuth(vID, 0, "Please add me.", 0);
	
	if( vTaskId != 0 )
		Add(EM_new EMNetworkSearchItem(vTaskId, EM_TASK_SEARCH_USER));

	return vTaskId;
}

uint64 EMNetworkUserHandler::DeleteUser(uint64 p_vID)
{
	TSonorkId vID;

	vID.Set(EMUserInfo::ConvertId(p_vID));

	return  EMNetworkEngine::Instance() -> GetAPI() -> DelUserAuth(vID, 0);
}

uint64 EMNetworkUserHandler::FindUser(uint64 p_vID, const char* p_vNick, bool p_vOnlineOnly)
{
	TSonorkUserDataNotes oUserNotes;
	DWORD vSearchFlags;
	DWORD vTaskId;
	EMNetworkSearchItem* opSearch = NULL;

	oUserNotes.Clear();
	
	oUserNotes.userData.SetUserInfoLevel(SONORK_USER_INFO_LEVEL_1,false);
	oUserNotes.userData.Clear(false);
	
	if(p_vID != 0)
	{
		oUserNotes.userData.userId.Set(EMUserInfo::ConvertId(p_vID));
		vSearchFlags = SONORK_FUNC_NF_SEARCH_USER_STATE_ANY;
	}
	else
	{
		oUserNotes.userData.userId.SetZero();
		oUserNotes.userData.alias.Set(p_vNick);
		oUserNotes.userData.name.Set("");
		oUserNotes.userData.email.Set("");

		vSearchFlags = SONORK_FUNC_NF_SEARCH_USER_STATE_ANY; //p_vOnlineOnly ? SONORK_FUNC_NF_SEARCH_USER_STATE_ONLINE : SONORK_FUNC_NF_SEARCH_USER_STATE_ANY;
	}

	eo << "EMNetworkUserHandler::FindUser - Searching for " << p_vNick << " " << (int64)p_vID << ef;

	vTaskId = EMNetworkEngine::Instance() -> GetAPI() -> SearchUser(&oUserNotes, vSearchFlags);

	if( vTaskId != 0 )
	{
		eo << "                               - Started: " << vTaskId << ef;
		opSearch = EM_new EMNetworkSearchItem(vTaskId, EM_TASK_SEARCH_USER);
		Add(opSearch);
	}
	else
		eo << "                               - Error"<< ef;

	return vTaskId;
}

void EMNetworkUserHandler::AddFoundUser(EMUserInfo* p_opUser)
{
	AddMessage(p_opUser);

	EMNetworkEngine::Instance() -> Notify(EM_NETWORK_USER_FOUND);
}


EMUserInfo* EMNetworkUserHandler::GetNextFound(uint64 p_vTask)
{
	return GetMessage();
}

void EMNetworkUserHandler::NotifySearchEnded(int64 p_vTaskId)
{
	EMNetworkEngine::Instance() -> Notify(EM_NETWORK_USER_SEARCH_ENDED);	
}

void EMNetworkUserHandler::NotifyUserAdded(int64 p_vTaskId)
{
	EMNetworkEngine::Instance() -> Notify(EM_NETWORK_USER_ADDED);	
}

void EMNetworkUserHandler::NotifyUserDeleted(int64 p_vTaskId)
{
	EMNetworkEngine::Instance() -> Notify(EM_NETWORK_USER_DELETED);	
}


void EMNetworkUserHandler::TimeSlot()
{
	LockContainer();
	Rewind();

	while(Current() != NULL)
	{
		if(Current() -> m_vPhase == EM_PHASE_DONE)
		{
			eo << "EMNetworkUserHandler::TimeSlot - Removing item: " << Current() -> GetId()   << ef;

			EMNetworkSearchItem* opInfo = Current();
			Remove(opInfo);
			delete opInfo;
			Rewind();
		}
		Next();
	}
	
/*	Rewind();
	while(Current() != NULL)
	{
		if(Current() -> m_opSession != NULL)
			Current() -> m_opSession -> TimeSlot();
		Next();
	}*/
	UnlockContainer();
}
