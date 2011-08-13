#include "EMNetworkUserDatabase.h"
#include "EMNetworkEngine.h"

#include "sonorkvcapi.h"

EMNetworkUserDatabase::EMNetworkUserDatabase()
  : EMItemContainer<EMUserInfo>()
{
}

EMNetworkUserDatabase::~EMNetworkUserDatabase()
{	
	ClearUsers();
}

void EMNetworkUserDatabase::GoOffline()
{
}


void EMNetworkUserDatabase::GoOnline()
{
}


const EMUserInfo* EMNetworkUserDatabase::GetFirstUser()
{
	EMUserInfo* opRet = NULL;

	Rewind();
	opRet = Current();
	Next();
	return opRet;
}



const EMUserInfo* EMNetworkUserDatabase::GetNextUser()
{
	EMUserInfo* opRet = NULL;

	if(Current() != NULL)
	{
		opRet = Current();
		Next();
	}

	return opRet;
}

uint32 EMNetworkUserDatabase::GetNumberOfUsers()
{
	return Size();
}


void EMNetworkUserDatabase::ClearUsers()
{
	EMUserInfo* oUser;

	LockContainer();
	while(Size() != 0)
	{
		Rewind();
		oUser = Current();
		Remove(oUser);
		delete oUser;
	}
	UnlockContainer();
}

void EMNetworkUserDatabase::AddUser(EMUserInfo* p_opUser)
{
	Add(p_opUser);
}


void EMNetworkUserDatabase::UpdateOnlineStatus(uint64 p_oUserId, TSonorkApiUserSid* p_opSid)
{
	EMUserInfo* opUser = Find(p_oUserId);

	eo << "EMNetworkUserDatabase::UpdateOnlineStatus" << ef;

	if(opUser != NULL)
	{
		eo << "EMNetworkUserDatabase::UpdateOnlineStatus - Got User" << ef;

		opUser -> GetHandle() -> addr.Set(p_opSid -> userAddr);

		opUser -> m_vOnline = opUser -> GetHandle() -> IsOnline();

		AddMessage(opUser);
	}
}

EMUserInfo* EMNetworkUserDatabase::GetNextStatusChange()
{
	return GetMessage();
}
