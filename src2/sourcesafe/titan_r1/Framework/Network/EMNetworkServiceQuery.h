
#ifndef __EM_NETWORK_SERVICE_QUERY
#define __EM_NETWORK_SERVICE_QUERY

#include "EMGlobals.h"
#include "srk_ipc.h"

class EMUserInfo;

class EMNetworkServiceQuery
{
public:
	EMNetworkServiceQuery();
	~EMNetworkServiceQuery();
	
	void Clear();
	void Wait();
	void WaitAndNotify();

	uint64 GetId();

	TSonorkServiceQuery m_oQuery;
	EMUserInfo* m_opUser;

	static void __cdecl Thread(void* p_vParam);

private:
	HANDLE m_oEvent;
};

#endif