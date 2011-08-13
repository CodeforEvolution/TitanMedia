#include "EMNetworkSearchItem.h"
#include "EMUserInfo.h"
#include "EMNetworkEngine.h"
#include "EMNetworkUserHandler.h"

#include "sonorkvcapi.h"

EMNetworkSearchItem::EMNetworkSearchItem(int64 p_vTaskId, EMTaskType p_vType)
  :	m_vTaskId(p_vTaskId),
	m_vType(p_vType),
	m_vPhase(EM_PHASE_WORKING)
{
}

int64 EMNetworkSearchItem::GetId() 
{ 
	return m_vTaskId;
}


void EMNetworkSearchItem::TaskEvent(TSonorkApiTaskEvent* p_opTask)
{
	TSonorkError oError;
	TSonorkUserDataNotes* oTaskNotes;
	EMUserInfo* opUser;

	if(p_opTask -> taskId != m_vTaskId) 
		return;

	if(p_opTask -> event == SONORK_CLIENT_REQUEST_EVENT_PACKET)
	{
		eo << "EMNetworkSearchItem::TaskEvent - SONORK_CLIENT_REQUEST_EVENT_PACKET" << ef;
		if(p_opTask -> packetSubFunction != 0)
			return;

		switch(m_vType)
		{
		case EM_TASK_SEARCH_USER:
			oTaskNotes = EMNetworkEngine::Instance() -> GetAPI() -> GetTaskUserDataNotes();

			if(oTaskNotes == NULL)
				return;

			opUser = EM_new EMUserInfo();

			opUser -> m_opName = string(oTaskNotes -> userData.alias.CStr());
			opUser -> m_opUID = EMUserInfo::ConvertId(oTaskNotes -> userData.userId);
			opUser -> m_vOnline = TSonorkApi::NormalizeSidMode(oTaskNotes -> userData.SidMode()) > 0;
			opUser -> m_vTaskId = m_vTaskId;

			eo << "EMNetworkSearchItem::TaskEvent - EM_TASK_SEARCH_USER" << ef;

			EMNetworkEngine::Instance() -> GetUserHandler() -> AddFoundUser(opUser);
			break;
		default:
			//Dont have to do anything with the rest at this point.
			break;
		}
	}
	else
	if(p_opTask -> event == SONORK_CLIENT_REQUEST_EVENT_END)
	{
		eo << "EMNetworkSearchItem::TaskEvent - SONORK_CLIENT_REQUEST_EVENT_END" << ef;

		oError.Set(p_opTask -> errorInfo);

		if(oError.result == SONORK_RESULT_OK )
		{
			switch(m_vType)
			{
			case EM_TASK_SEARCH_USER:
				eo << "EMNetworkSearchItem::TaskEvent - EM_TASK_SEARCH_USER / Search Ended" << ef;

				EMNetworkEngine::Instance() -> GetUserHandler() -> NotifySearchEnded(m_vTaskId);
				break;
			case EM_TASK_ADD_USER:
				eo << "EMNetworkSearchItem::TaskEvent - EM_TASK_ADD_USER" << ef;

				EMNetworkEngine::Instance() -> GetUserHandler() -> NotifyUserAdded(m_vTaskId);
				break;
			case EM_TASK_DELETE_USER:
				eo << "EMNetworkSearchItem::TaskEvent - EM_TASK_DELETE_USER" << ef;

				EMNetworkEngine::Instance() -> GetUserHandler() -> NotifyUserDeleted(m_vTaskId);
				break;
			}
			m_vPhase = EM_PHASE_DONE;
		}
		else //Error...
			m_vPhase = EM_PHASE_ERROR;
	}
}

