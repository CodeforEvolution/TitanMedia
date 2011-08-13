#include "EMNetworkEngine.h"
#include "EMUserInfo.h"
#include "EMNetworkUserDatabase.h"
#include "EMNetworkCommandFactory.h"

#include "EMNetworkMessage.h"
#include "EMNetworkMessageSender.h"
#include "EMNetworkMessageQueue.h"
#include "EMNetworkFileHandler.h"
#include "EMNetworkUserHandler.h"
#include "EMSemaphore.h"

/*
#include "EMUserProfile.h"
#include "EMNetworkFileId.h"
#include "EMNetworkFileInfo.h"
#include "EMNetworkServiceModule.h"*/

#include "sonorkvcapi.h"

#include <windows.h>
#include <process.h>

#include <stdio.h>

#include "EMMessages.h"

EMNetworkEngine* EMNetworkEngine::m_opInstance = NULL;
bool EMNetworkEngine::m_vIsDecomposed = false;

EMNetworkEngine::EMNetworkEngine()
 : m_opUsers(NULL),
   m_opSender(NULL),
   m_opMessageQueue(NULL),
   m_opFileHandler(NULL),
   m_opUserHandler(NULL),
   m_vIsRunning(true),
   m_oCurrentPhase(EM_PHASE_NONE),
   m_oLastPhase(EM_PHASE_NONE)
{
	m_opAPI = new TSonorkApi(MainAPICallback, (DWORD)this);

	m_oThreadEnd = CreateEvent(NULL, false, false, NULL);
	m_oUsersDownloaded = CreateEvent(NULL, false, false, NULL);

	m_opLock = EMSemaphore::CreateEMSemaphore("NetworkEngineProtectionSemaphore", 1, 1);

	_beginthread(EMNetworkEngine::TimerThread, 0, this);
}

EMNetworkEngine::~EMNetworkEngine()
{
	m_opLock -> Acquire();
	m_vIsRunning = false;
	if(m_opAPI != NULL)
	{
		m_opAPI -> Disconnect();
		delete m_opAPI;
	}
	m_opLock -> Release();

	m_vIsDecomposed = true;

	WaitForSingleObject(m_oThreadEnd, INFINITE);

	CloseHandle(m_oThreadEnd);
	CloseHandle(m_oUsersDownloaded);

	EMNetworkCommandFactory::Delete();

	delete m_opLock;
}


void EMNetworkEngine::TimerThread(void *p_vpThis)
{
	bool vDidRun = false;
	EMNetworkEngine* opMain = static_cast<EMNetworkEngine*>(p_vpThis);

	opMain -> m_opLock -> Acquire();

	while(opMain -> m_vIsRunning)
	{
		vDidRun = true;
		if(opMain -> GetAPI() != NULL)
		{
			opMain -> GetAPI() -> TimeSlot();
		}

		if(opMain -> m_vIsRunning && opMain -> m_oCurrentPhase != opMain -> m_oLastPhase)
		{ 
			opMain -> PhaseTransition();
		}

		EMNetworkMessageSender* opSender = opMain -> GetMessageSender();
		if(opSender != NULL)
		{
			opSender -> TimeSlot();
		}

		EMNetworkFileHandler* opFile = opMain -> GetFileHandler();
		if(opFile != NULL)
		{
			opFile -> TimeSlot();
		}

		EMNetworkUserHandler* opUser = opMain -> GetUserHandler();
		if(opUser != NULL)
		{
			opUser -> TimeSlot();
		}

		try
		{
			opMain -> m_opLock -> Release();
			Sleep(200);
		}
		catch(...)
		{
			opMain -> m_opLock -> Release();
		}
	}
	
	if(! vDidRun)
		opMain -> m_opLock -> Release();

	SetEvent(opMain -> m_oThreadEnd);
}

void EMNetworkEngine::PhaseTransition()
{
	m_oLastPhase = m_oCurrentPhase;

	switch( m_oCurrentPhase )
	{
	case EM_PHASE_DOWNLOADING_USERS:
		m_opAPI -> RefreshUserList();
		break;

	case EM_PHASE_UPDATING_SID:
		m_opAPI -> SetSidMode(SONORK_SID_MODE_ONLINE,"You sank my battleship!!");
		break;

	default:
		return;

	}

	if(m_opAPI -> GetLastResult() != SONORK_RESULT_OK)
	{
		m_opAPI -> Disconnect();
	}
}

void EMNetworkEngine::Delete()
{
	if(m_opInstance != NULL)
	{
		m_opInstance -> Disconnect();
		delete m_opInstance;
		m_opInstance = NULL;
	}

	m_vIsDecomposed = true;
}

EMNetworkEngine* EMNetworkEngine::Instance()
{

	if(m_opInstance != NULL)
		return m_opInstance;

	if(m_vIsDecomposed)
	{
		return 0;
	}

	if(m_opInstance == NULL)
		m_opInstance = EM_new EMNetworkEngine();

	EMNetworkEngine* opInstance = m_opInstance;

	return opInstance;	
}


bool EMNetworkEngine::Initialize(const char* p_opHost, uint32 m_vPort, bool p_vTCP, bool p_vIntranet)
{
	SONORK_PHYS_ADDR_TYPE vAddrType;
	SONORK_LOGIN_MODE	  vLoginMode;

	if(m_opAPI == NULL)
		return false;

	if(m_opAPI -> GetNetworkStatus() != SONORK_NETIO_STATUS_DISCONNECTED)
	{
		return false;
	}

	vAddrType = p_vTCP ? SONORK_PHYS_ADDR_TCP_1 : SONORK_PHYS_ADDR_UDP_1;
	vLoginMode = !p_vIntranet ? SONORK_LOGIN_MODE_INTRANET : SONORK_LOGIN_MODE_INTERNET;

	m_opAPI -> SetSonorkServer(vAddrType , p_opHost, m_vPort, vLoginMode);

	return m_opAPI -> GetLastResult() == SONORK_RESULT_OK;
}

bool EMNetworkEngine::Connect(const char* p_opUID, const char* p_opPWD)
{
	SONORK_DWORD2	oUserId;
	
	if(m_opAPI == NULL)
		return false;

	if(m_opAPI -> GetNetworkStatus() != SONORK_NETIO_STATUS_DISCONNECTED)
	{
		return true;
	}

	if(p_opUID != NULL)
	{
		if(!oUserId.SetStr(p_opUID))
			return false;
	}
	else
	{
		if(!oUserId.SetStr("1000.4"))
			return false;
	}

	if(p_opPWD != NULL)
		m_opAPI -> Connect(&oUserId, p_opPWD);
	else
		m_opAPI -> Connect(&oUserId, "MoD");

	if(m_opAPI -> GetLastResult() != SONORK_RESULT_OK)
		return false;

	
	SONORK_NETIO_STATUS oStat = m_opAPI -> GetNetworkStatus();


	if(m_opAPI -> GetNetworkStatus() == SONORK_NETIO_STATUS_DISCONNECTED)
		return false;

	if(m_opUsers != NULL)
		delete m_opUsers;
	m_opUsers = new EMNetworkUserDatabase();

	if(m_opSender != NULL)
		delete m_opSender;	 
	m_opSender = new EMNetworkMessageSender();

	if(m_opMessageQueue != NULL)
		delete m_opMessageQueue;
	m_opMessageQueue = new EMNetworkMessageQueue<EMNetworkMessage*>;

/*	if(m_opProfile != NULL)
		delete m_opProfile;
	m_opProfile = new EMUserProfile(m_opIpc);
*/


	if(m_opFileHandler != NULL)
		delete m_opFileHandler;
	m_opFileHandler = new EMNetworkFileHandler();

	if(m_opUserHandler != NULL)
		delete m_opUserHandler;
	m_opUserHandler = new EMNetworkUserHandler();


	WaitForSingleObject(m_oUsersDownloaded, 500);

	return 	true;
}

bool EMNetworkEngine::Disconnect()
{
	if(m_opAPI == NULL)
		return true;

	if(m_opAPI -> GetNetworkStatus() == SONORK_NETIO_STATUS_DISCONNECTED)
	{
		return true;
	}

	m_opAPI -> Disconnect();


	if(m_opUsers != NULL)
	{
		delete m_opUsers;
		m_opUsers = NULL;
	}

	if(m_opSender != NULL)
	{
		delete m_opSender;	
		m_opSender = NULL;
	}

	if(m_opMessageQueue != NULL)
	{
		delete m_opMessageQueue;
		m_opMessageQueue = NULL;
	}

	if(m_opFileHandler != NULL)
	{
		delete m_opFileHandler;
		m_opFileHandler = NULL;
	}

	if(m_opUserHandler != NULL)
	{
		delete m_opUserHandler;
		m_opUserHandler = NULL;
	}

	return true;
}


bool EMNetworkEngine::IsConnected()
{
	if(m_opAPI != NULL)
		return m_opAPI -> GetNetworkStatus() != SONORK_NETIO_STATUS_DISCONNECTED;

	return false;
}	


//_---------------------------------_______------------------------------------------


void EMNetworkEngine::MainAPICallback(TSonorkApi* p_opAPI)
{
	TSonorkApiTaskEvent	oTask;
	EMNetworkTask* opInfo;

	EMNetworkEngine* opThis = reinterpret_cast<EMNetworkEngine*>(p_opAPI -> HandlerTag());

	switch(p_opAPI -> ApiEvent())
	{
	case SONORK_API_EVENT_CLIENT:
		eo << "EMNetworkEngine::MainAPICallback - SONORK_API_EVENT_CLIENT Begin" << ef;
		opThis -> ClientEvent();
		eo << "EMNetworkEngine::MainAPICallback - SONORK_API_EVENT_CLIENT End" << ef;
		break;

	case SONORK_API_EVENT_TASK:
		eo << "EMNetworkEngine::MainAPICallback - SONORK_API_EVENT_TASK Begin" << ef;

		if(p_opAPI -> GetTaskEvent(&oTask) != SONORK_RESULT_OK)
			break;


		eo << "EMNetworkEngine::MainAPICallback - " << oTask.taskId << ef;

		
		opInfo = opThis -> m_opFileHandler -> Find(oTask.taskTag);
		if(opInfo == NULL)
		{
			opInfo = opThis -> m_opSender -> Find(oTask.taskTag);
		}
		if(opInfo == NULL)
		{
			opInfo = opThis -> m_opUserHandler -> Find(oTask.taskId);
		}

		if(opInfo != NULL)
		{
			eo << "EMNetworkEngine::MainAPICallback - Found:" << oTask.taskId << ef;
			opInfo -> TaskEvent(&oTask);
		}
		else
			eo << "EMNetworkEngine::MainAPICallback - Lost:" << oTask.taskId << ef;


		eo << "EMNetworkEngine::MainAPICallback - SONORK_API_EVENT_TASK End" << ef;

		break;
	default:
		break;
	}
}



//-----------------------------------------------------------------------------------

void EMNetworkEngine::ClientEvent()
{
	TSonorkApiClientEvent	oEvent;

	if(m_opAPI -> GetClientEvent(&oEvent) != SONORK_RESULT_OK)
		return;

	switch(oEvent.event)
	{
	case SONORK_CLIENT_EVENT_STATUS_CHANGE:
		eo << "EMNetworkEngine::ClientEvent SONORK_CLIENT_EVENT_STATUS_CHANGE" << ef;
		StatusChanged((SONORK_NETIO_STATUS)oEvent.eventData[0], (SONORK_NETIO_STATUS)oEvent.eventData[1], oEvent.eventData[2], oEvent.errorInfo);
		break;

	case SONORK_CLIENT_EVENT_GLOBAL_TASK:
		eo << "EMNetworkEngine::ClientEvent SONORK_CLIENT_EVENT_GLOBAL_TASK" << ef;
		GlobalTask((SONORK_FUNCTION)oEvent.eventData[0], oEvent.eventData[1] ? true : false, oEvent.errorInfo);
		break;

	case SONORK_CLIENT_EVENT_USER_ADD:
		eo << "EMNetworkEngine::ClientEvent SONORK_CLIENT_EVENT_USER_ADD" << ef;
		AddUserFromNetwork();
		break;

	case SONORK_CLIENT_EVENT_USER_DEL:
		eo << "EMNetworkEngine::ClientEvent SONORK_CLIENT_EVENT_USER_DEL" << ef;
		DeleteUser(oEvent.objectId);
		break;

	case SONORK_CLIENT_EVENT_USER_AUTH:
		eo << "EMNetworkEngine::ClientEvent SONORK_CLIENT_EVENT_USER_AUTH" << ef;
		AuthorizeUser(oEvent.objectId);
		break;

	case SONORK_CLIENT_EVENT_GROUP_ADD:
		eo << "EMNetworkEngine::ClientEvent SONORK_CLIENT_EVENT_GROUP_ADD" << ef;
		//Can be used as collabortaion groups??
		break;

	case SONORK_CLIENT_EVENT_USER_SET:
		eo << "EMNetworkEngine::ClientEvent SONORK_CLIENT_EVENT_USER_SET" << ef;
		break;

	case SONORK_CLIENT_EVENT_USER_SID:
		eo << "EMNetworkEngine::ClientEvent SONORK_CLIENT_EVENT_USER_SID" << ef;
		SidChanged(oEvent.objectId);
		break;

	case SONORK_CLIENT_EVENT_MSG:
		eo << "EMNetworkEngine::ClientEvent SONORK_CLIENT_EVENT_MSG" << ef;
		MessageReceived(oEvent.objectId);
		break;
	}
}

void EMNetworkEngine::DeleteUser(const SONORK_DWORD2& p_oReqUserId)
{
	int64 vID = EMUserInfo::ConvertId(p_oReqUserId);

	EMUserInfo* opUser = GetUserDatabase() -> Find(vID);

	if(opUser != NULL)
	{
		GetUserDatabase() -> Remove(opUser);
		delete opUser;
		
		Notify(EM_NETWORK_USER_DELETED);
	}
}

void EMNetworkEngine::AuthorizeUser(const SONORK_DWORD2& p_oReqUserId)
{
	TSonorkUserDataNotes oUserNotes;

	if(GetAPI() -> GetEventUser(&oUserNotes) != SONORK_RESULT_OK)
		return;

	DeleteUser(oUserNotes.userData.userId);

	if(p_oReqUserId == GetAPI() -> User().userId)
		return;	// We generated this event, ignore

	oUserNotes.auth.Clear();
	oUserNotes.auth.flags.SetUserInfoLevel(SONORK_USER_INFO_LEVEL_1);

	GetAPI() -> ReplyUserAuth(oUserNotes.userData.userId, oUserNotes.auth, 0);
}


void EMNetworkEngine::StatusChanged(SONORK_NETIO_STATUS p_oNewStatus, SONORK_NETIO_STATUS p_oOldStatus, DWORD p_vFlags, TSonorkError& p_vError)
{
    if(p_oNewStatus > SONORK_NETIO_STATUS_DISCONNECTED && p_oOldStatus <= SONORK_NETIO_STATUS_DISCONNECTED)
	{
		m_oCurrentPhase = EM_PHASE_CONNECTING;
	}
	else if(p_oNewStatus <= SONORK_NETIO_STATUS_DISCONNECTED && p_oOldStatus > SONORK_NETIO_STATUS_DISCONNECTED)
	{
		m_oCurrentPhase = m_oLastPhase = EM_PHASE_NONE;
		eo << "Sending lost connectin notify!!" << ef;
		Notify(EM_NETWORK_LOST_CONNECTION);
	}
	else if(p_oNewStatus >= SONORK_NETIO_STATUS_CONNECTED && p_oOldStatus < SONORK_NETIO_STATUS_CONNECTED )
	{
		m_oCurrentPhase = EM_PHASE_DOWNLOADING_USERS;        
    }
}

void EMNetworkEngine::GlobalTask(SONORK_FUNCTION p_oTask, bool p_vDone, TSonorkError& p_oError)
{
    if(p_vDone)
	{
        switch(p_oTask)
		{
		case SONORK_FUNCTION_SET_SID:
			eo << "EMNetworkEngine::GlobalTask - SONORK_FUNCTION_SET_SID(2)" << ef;

            if(p_oError.result != SONORK_RESULT_OK)
			{
				m_opAPI -> Disconnect();
			}
			else if(m_oLastPhase == EM_PHASE_UPDATING_SID)
			{
				m_oCurrentPhase = EM_PHASE_DONE;
				return;
            }
			break;
                
        case SONORK_FUNCTION_USER_LIST:
			eo << "EMNetworkEngine::GlobalTask - SONORK_FUNCTION_USER_LIST(2)" << ef;

            if(p_oError.result != SONORK_RESULT_OK)
			{
                m_opAPI -> Disconnect();
			}
			else if(m_oLastPhase == EM_PHASE_DOWNLOADING_USERS)
			{
				SetEvent(m_oUsersDownloaded);
				m_oCurrentPhase = EM_PHASE_UPDATING_SID;
				return;
            }
			break;            
		}
        
	}
	else
	{
        switch(p_oTask)
		{
        case SONORK_FUNCTION_SET_SID:
			eo << "EMNetworkEngine::GlobalTask - SONORK_FUNCTION_SET_SID" << ef;
			break;            
                
        case SONORK_FUNCTION_USER_LIST:
			eo << "EMNetworkEngine::GlobalTask - SONORK_FUNCTION_USER_LIST" << ef;
			break;                                
        }        
	}
}

void EMNetworkEngine::AddUserFromNetwork()
{
	TSonorkUserData* opUser;
	SONORK_SID_MODE	 vSidMode;

	TSonorkAuth2*		oAuthPtr = NULL;
	TSonorkDynString*	oNotes = NULL;

	opUser = m_opAPI -> GetEventUser(&oAuthPtr, &oNotes);

	if(opUser == NULL)
		return;

	vSidMode = TSonorkApi::NormalizeSidMode(opUser -> SidMode());

	EMUserInfo* opInfo = EM_new EMUserInfo(opUser, vSidMode != SONORK_SID_MODE_DISCONNECTED);

	m_opUsers -> Add(opInfo);

	eo << "EMNetworkEngine::AddUserFromNetwork: " << opUser -> alias.CStr() << ". Notifying Johan..." << ef;
	Notify(EM_NETWORK_USER_ADDED);
}

void EMNetworkEngine::SidChanged(const SONORK_DWORD2& userId)
{
	TSonorkApiUserSid oSid;

	if(m_opAPI -> GetEventSid(&oSid) != SONORK_RESULT_OK)
		return;

	m_opUsers -> UpdateOnlineStatus(EMUserInfo::ConvertId(userId), &oSid);
	Notify(EM_NETWORK_USER_CHANGED);
}




void EMNetworkEngine::MessageReceived(const SONORK_DWORD2& userId)
{
	TSonorkMsg* opMsg;

	opMsg = m_opAPI -> GetEventMsg();

	if(opMsg == NULL)
		return;

	if(opMsg -> DataServiceType() == SONORK_SERVICE_TYPE_SONORK_FILE)
	{
		eo << "EMNetworkEngine::MessageReceived - We have a file... " << ef;
		m_opFileHandler -> AddFile(opMsg);

		Notify(EM_NETWORK_FILE_ARRIVED);
	}
	else
	{
		EMNetworkMessage* opEMMsg = new EMNetworkMessage(EMUserInfo::ConvertId(userId), opMsg);
		m_opMessageQueue -> AddMessage(opEMMsg);

		Notify(EM_NETWORK_MESSAGE_ARRIVED);
	}	
}

//_--------------------------------_______------------------------------------------


EMNetworkUserDatabase* EMNetworkEngine::GetUserDatabase()
{
	return m_opUsers;
}

EMNetworkMessageSender* EMNetworkEngine::GetMessageSender()
{
	return m_opSender;
}

EMNetworkMessageQueue<EMNetworkMessage*>* EMNetworkEngine::GetMessageQueue()
{
	return m_opMessageQueue;
}

EMNetworkFileHandler* EMNetworkEngine::GetFileHandler()
{
	return m_opFileHandler;
}

EMNetworkUserHandler* EMNetworkEngine::GetUserHandler()
{
	return m_opUserHandler;
}


EMNetworkCommandFactory* EMNetworkEngine::GetCommandFactory()
{
	return EMNetworkCommandFactory::Instance();
}


TSonorkApi* EMNetworkEngine::GetAPI()
{
	return m_opAPI;
}

