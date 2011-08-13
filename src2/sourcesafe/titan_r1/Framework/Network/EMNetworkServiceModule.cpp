#include "EMNetworkServiceModule.h"
#include "EMNetworkServiceQuery.h"
#include "EMNetworkEngine.h"
#include "EMNetworkUserDatabase.h"
#include "EMNetworkFileHandler.h"
#include "EMMessages.h"

#include <windows.h>
#include <process.h>

#include "srkwin32app_defs.h"

EMNetworkServiceModule::EMNetworkServiceModule(TSonorkIpc* p_opIpc) 
: m_opIpc(p_opIpc)
{
	Register();
}

EMNetworkServiceModule::~EMNetworkServiceModule()
{
	UnRegister();
}

bool EMNetworkServiceModule::Register()
{
	SONORK_RESULT	result;

	m_oServiceInfo.SetInstanceInfo(SONORK_SERVICE_ID_DEMO_APP, SONORK_SERVICE_TYPE_NONE, 0, 0, 1);
	result = m_opIpc -> RegisterService(SONORK_APP_SERVICE_TYPE_SERVER, &m_oServiceInfo, 0);

	if(result != SONORK_RESULT_OK)
	{
		eo <<"EMNetworkServiceModule::Register - Couldn't register!!" <<ef;
	}

	return result == SONORK_RESULT_OK;
}

bool EMNetworkServiceModule::UnRegister()
{
	if(m_oServiceInfo.ServiceInstance() == 0)
		return true;

	m_opIpc -> UnregisterService(m_oServiceInfo.ServiceInstance());
	m_oServiceInfo.Clear();

	return true;
}

DWORD EMNetworkServiceModule::HandleRequest(DWORD p_vEventId, TSonorkIpcSvcEvent& p_oSvcEvent)
{
	EMNetworkServiceQuery* opQuery;
	TSonorkUserServer userver;

	switch(p_vEventId)
	{

	case SONORK_APP_SERVICE_EVENT_GET_NAME:
		p_oSvcEvent.getName.str -> Set("Titan");
		return true;

	case SONORK_APP_SERVICE_EVENT_GET_SERVER_DATA:
	{
		TSonorkUserServer oData;

		oData.CODEC_Clear();
		oData.SetInstanceInfo(m_oServiceInfo);
		oData.SetState(SONORK_SERVICE_STATE_READY);
		oData.name.Set("Titan");
		oData.text.Set("Blaha Blaha");

		SONORK_RESULT result = m_opIpc -> SetServerData(&oData);

		return result == SONORK_RESULT_OK;
	}

	case SONORK_APP_SERVICE_EVENT_QUERY_AKN:
		// We're getting answers for a query started with IPC::StartServiceQuery()
		// as shown in "services.cpp".
		// The instance that requested the query is in <svcEvent.serviceInstance>
		// The event tag (passed to ServiceStartQuery) is in  <svcEvent.eventTag>
		// The handler tag (passed to RegisterService) is in <svcEvent.handlerTag>
		// The <svcEvent.msgInfo> contains the information about the query
		// The id of the query is in <svcEvent.msgInfo.ctrlMsg->QueryId()>
		// <svcEvent.msgInfo.serviceHandle> is the handle of the service replying
		// <svcEvent.msgInfo.ctrlMsg> is the control message sent to us as reply
		// <svcEvent.msgInfo.dataSize> is the size of the data sent.
		// if <dataSize> is not zero, the data may be obtained
		// with IPC::GetCurrentServiceData()
		unsigned long buffSize;
		unsigned char* buffer;

		if(p_oSvcEvent.msgInfo.ctrlMsg -> Cmd() != SONORK_APP_CTRLMSG_QUERY_SERVICES)
			break;

		// Is the DATA flag set?
		if(!(p_oSvcEvent.msgInfo.ctrlMsg -> CmdFlags() & SONORK_CTRLMSG_CF_DATA))
			break;

		buffSize = p_oSvcEvent.msgInfo.dataSize;
		buffer = new unsigned char[buffSize + 2];

		opQuery = Find(p_oSvcEvent.msgInfo.ctrlMsg -> QueryId());

		eo << "SONORK_APP_SERVICE_EVENT_QUERY_AKN??" <<ef;

		if(m_opIpc -> GetServiceData(buffer, buffSize) == SONORK_RESULT_OK && opQuery != NULL)
		{
			if(userver.CODEC_ReadMem(buffer, buffSize) == SONORK_RESULT_OK)
			{
				if(strcmp(userver.name.CStr(), "Titan") == 0)
				{
					opQuery -> m_opUser -> m_oServiceInstance = userver.ServiceInstance();

					eo << "SONORK_APP_SERVICE_EVENT_QUERY_AKN!!" <<ef;

					Remove(opQuery);

					EMNetworkEngine::Instance() -> GetUserDatabase() -> UpdateServiceStatus(opQuery -> m_opUser -> GetId());
					EMNetworkEngine::Instance() -> Notify(EM_NETWORK_USER_CHANGED);

					opQuery -> Clear();
				}
			}
		}

		delete[] buffer;
		break;

	case SONORK_APP_SERVICE_EVENT_QUERY_END:
		opQuery = Find(p_oSvcEvent.close.systemId);
		if(opQuery != NULL)
		{
			eo << "SONORK_APP_SERVICE_EVENT_QUERY_END!!" <<ef;
			opQuery -> m_opUser -> m_oServiceInstance = -1;
			opQuery -> Clear();
		}
		break;

	case SONORK_APP_SERVICE_EVENT_POKE_DATA:
		// Another service is sending us data using the
		// IPC::PokeServiceData.
		{
			TSonorkDynString	string;
			char buff[256];

			if(p_oSvcEvent.msgInfo.ctrlMsg -> Cmd() != SONORK_CTRLMSG_CMD_01)
			{
				break;
			}

			if(m_opIpc -> GetServiceDataAtom(&string) == SONORK_RESULT_OK)
			{
				sprintf(buff, "%s",string.CStr());

				eo << "Somebody poked us!!!   " << buff <<ef;

				if(buff[0] == '|')
				{
					EMNetworkEngine::Instance() -> GetFileHandler() -> MarkFileForMediaPool(buff + 1);
				}
				else if(buff[0] == 'd')
				{
					EMUserInfo* opUser = EMNetworkEngine::Instance() -> GetUserDatabase() -> Find(EMUserInfo::ConvertId(p_oSvcEvent.msgInfo.serviceHandle->userId));

					if(opUser != NULL)
					{
						opUser -> m_oServiceInstance = -1;

						EMNetworkEngine::Instance() -> GetUserDatabase() -> UpdateServiceStatus(opUser -> GetId());
						EMNetworkEngine::Instance() -> Notify(EM_NETWORK_USER_CHANGED);
					}
				}
				else if(buff[0] == 'c')
				{
					EMUserInfo* opUser = EMNetworkEngine::Instance() -> GetUserDatabase() -> Find(EMUserInfo::ConvertId(p_oSvcEvent.msgInfo.serviceHandle -> userId));

					if(opUser != NULL)
					{
						opUser -> m_oServiceInstance = p_oSvcEvent.msgInfo.serviceHandle -> instance;

						EMNetworkEngine::Instance() -> GetUserDatabase() -> UpdateServiceStatus(opUser -> GetId());
						EMNetworkEngine::Instance() -> Notify(EM_NETWORK_USER_CHANGED);
					}
				}
			}
		}
		break;
	default:
		break;
	}
	
	return 0;
}

unsigned EMNetworkServiceModule::AskThread(void *p_vpThis)
{
	EMNetworkServiceQuery* opQuery = static_cast<EMNetworkServiceQuery*>(p_vpThis);
	EMNetworkServiceModule* opService = EMNetworkEngine::Instance() -> GetServiceModule();

	SONORK_RESULT result = opService -> m_opIpc -> StartServiceQuery(opService -> m_oServiceInfo.ServiceInstance(), &(opQuery -> m_oQuery), SONORK_APP_CTRLMSG_QUERY_SERVICES, 0, 0, 15000, NULL, 0, NULL);

	return 0;
}


bool EMNetworkServiceModule::QueryRemoteServices(EMUserInfo* p_opUser, bool p_vNotifyRemote)
{
	EMNetworkServiceQuery* opQuery = new EMNetworkServiceQuery();

	opQuery -> m_opUser = p_opUser;

	opQuery -> m_oQuery.userId.v[0] = p_opUser -> GetLowId();
	opQuery -> m_oQuery.userId.v[1] = p_opUser -> GetHighId();

	if(m_opIpc -> GetUserHandle(&(opQuery -> m_oQuery)) != SONORK_RESULT_OK)
	{
		return false;
	}

	char buff[256];
	memcpy(buff, p_opUser -> m_opName.c_str(), p_opUser -> m_opName.length() + 1);

//	eo << "EMNetworkServiceModule::QueryRemoteServices - Peeking at: " << buff << ef;

	opQuery -> m_oQuery.LoadTarget(SONORK_SERVICE_ID_SONORK, SONORK_SERVICE_LOCATOR_INSTANCE(0));

	Add(opQuery);

	SONORK_RESULT result = m_opIpc -> StartServiceQuery(m_oServiceInfo.ServiceInstance(), &(opQuery -> m_oQuery), SONORK_APP_CTRLMSG_QUERY_SERVICES, 0, 0, 15000, NULL, 0, NULL);


	if(result != SONORK_RESULT_OK)
	{
		Remove(opQuery);

//		eo << "EMNetworkServiceModule::QueryRemoteServices - Failed peeking at: " << buff << ef;

		delete opQuery;
		return false;
	}

	if(p_vNotifyRemote)
	{
		opQuery -> WaitAndNotify();
	}

	return true;
}


bool EMNetworkServiceModule::SendServiceData(EMUserInfo* p_opUser, char* p_vpMsg)
{
	TSonorkId oUID;
	TSonorkServiceHandle oRemoteHandle;
	TSonorkDynString oServiceMessage;

	oUID.v[0] = p_opUser -> GetLowId();
	oUID.v[1] = p_opUser -> GetHighId();

	oRemoteHandle.userId = oUID;

	if(m_opIpc -> GetUserHandle(&oRemoteHandle) != SONORK_RESULT_OK)
	{
		return false;
	}

	oRemoteHandle.instance = p_opUser -> m_oServiceInstance;

	oServiceMessage.Set(p_vpMsg);

	return SONORK_RESULT_OK == m_opIpc -> PokeServiceData(m_oServiceInfo.ServiceInstance(), &oRemoteHandle, SONORK_CTRLMSG_CMD_01, 0, 0, &oServiceMessage);
}
