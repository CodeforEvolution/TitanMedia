/*******************************************************
* Creator: Martin Johansson
* Portability: Native, very native
*-------------------------------------------------------
* 
* Martin Johansson, 01-08-01
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_NETWORK_SERVICE_MODULE
#define __EM_NETWORK_SERVICE_MODULE

#include <windows.h>
#include "srk_ipc.h"
#include "EMUserInfo.h"
#include "EMItemContainer.h"
#include "EMNetworkMessageQueue.h"
#include "EMNetworkEngine.h"

#include  "EMNetworkServiceQuery.h"


class EMNetworkServiceModule : public EMItemContainer<EMNetworkServiceQuery>
{
public:
	EMNetworkServiceModule(TSonorkIpc* p_opIpc);
	~EMNetworkServiceModule();

	bool Register();
	bool UnRegister();

	DWORD HandleRequest(DWORD p_vEventId, TSonorkIpcSvcEvent& p_oSvcEvent);

	bool SendServiceData(EMUserInfo* p_opUser, char* p_vpMsg);

	bool QueryRemoteServices(EMUserInfo* p_opUser, bool p_vNotifyRemote = false);

	
	static unsigned __stdcall AskThread(void *p_vpThis);
protected:
	TSonorkIpc* m_opIpc;
	TSonorkServiceInstanceInfo m_oServiceInfo;

private:
	EMNetworkServiceModule();
};

#endif
