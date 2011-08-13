#include "NetworkInfoCommandGetUserInfos.h"
#include "EMNetworkEngine.h"
#include "EMUserInfo.h"
#include "EMNetworkUserDatabase.h"


NetworkInfoCommandGetUserInfos::NetworkInfoCommandGetUserInfos()
  : m_vNum(0)
{
}

NetworkInfoCommandGetUserInfos::~NetworkInfoCommandGetUserInfos()
{
}

void* NetworkInfoCommandGetUserInfos::ExecuteE(void* p_opUIDs, void* p_opNames, void* p_opStatus)
{
	const EMUserInfo* opUser;
	uint64* vpUIDs = static_cast<uint64*>(p_opUIDs);
	uint32* vpStatus = static_cast<uint32*>(p_opStatus);
	const char**  vppNames = static_cast<const char**>(p_opNames);
	
	if(EMNetworkEngine::Instance() -> IsConnected())
	{
		opUser = EMNetworkEngine::Instance() -> GetUserDatabase() -> GetFirstUser();

		int i = 0;
		while(opUser != NULL)
		{
			vpUIDs[i]   = opUser -> m_opUID;
			vppNames[i] = opUser -> m_opName.c_str();
			vpStatus[i] = opUser -> m_vOnline;

			opUser = EMNetworkEngine::Instance() -> GetUserDatabase() -> GetNextUser();

			++i;
		}
	}

	return NULL;
}

bool NetworkInfoCommandGetUserInfos::IsUndoable()
{
	return false;
}

bool NetworkInfoCommandGetUserInfos::RequiresParameters()
{
	return true;
}
