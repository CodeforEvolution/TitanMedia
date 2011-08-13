#include "EMNetworkSendFileInfo.h"
#include "EMNetworkEngine.h"

#include "sonorkvcapi.h"

EMNetworkSendFileInfo::EMNetworkSendFileInfo(const char* p_opFileName)
{
	const char* oTmp = p_opFileName;

	if(*oTmp == '|')
	{
		++oTmp;
		m_vIsMediaMaterial = true;
	}

	char* oName = strrchr(oTmp, '\\');

	if(oName == NULL)
	{
		m_oFilePath = "";
		m_oFileName = oTmp;
	}
	else
	{
		*oName = 0;

		m_oFilePath = oTmp;

		if(m_vIsMediaMaterial)
		{
			*oName = '|';
			m_oFileName = oName;
		}
		else
		{
			m_oFileName = oName + 1;
		}

		*oName = '\\';
	}

	m_hFile = CreateFile(oTmp, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(m_hFile == INVALID_HANDLE_VALUE)
	{
		EMDebugger("EMNetworkSendFileInfo::EMNetworkSendFileInfo() - Error, file does not exist!!!");
	}

	m_vSize = GetFileSize(m_hFile, NULL);
}


EMNetworkSendFileInfo::~EMNetworkSendFileInfo()
{
}

void EMNetworkSendFileInfo::PhaseChange()
{
	if(m_opSession -> TxPhase() == SONORK_API_FILE_PHASE_CONNECTED)
	{
		SONORK_DWORD2List	oList;
		TSonorkFileInfo		srkInfo;
		SONORK_DWORD2		oID;

		oID.v[0] = m_opRemoteUser -> GetLowId();
		oID.v[1] = m_opRemoteUser -> GetHighId();

		oList.AddItem(&oID);

		srkInfo.Clear();
		srkInfo.attr.orig_size.v[0] = m_vSize;
		srkInfo.attr.stor_size.v[0] = m_vSize;
		srkInfo.name.Set(m_oFileName.c_str());

		SONORK_RESULT result = m_opSession -> RequestPutFile(&oList, &srkInfo, NULL);

		if(result != SONORK_RESULT_OK)
		{
			m_vPhase = EM_FILE_PHASE_ERROR;		
		}

		m_vPhase = EM_FILE_PHASE_SENDING;
	}
	else
	if(m_opSession -> TxPhase() == SONORK_API_FILE_PHASE_COMPLETE)
	{
		m_vPhase = EM_FILE_PHASE_DONE;		
	}
}


void EMNetworkSendFileInfo::TransferData()
{
	DWORD vCnt;

	if(!ReadFile(m_hFile, m_opSession -> DataBlockPtr(), m_opSession -> DataBlockSize(), &vCnt, NULL))
	{
		m_vPhase = EM_FILE_PHASE_ERROR;
	}


	eo << "Transfering data for " << m_oFileName.c_str() << ef;

	m_vTransfered += vCnt;
}

void EMNetworkSendFileInfo::TaskEvent(TSonorkApiTaskEvent* p_opTask)
{
/*	char vDummy[64];
	
	eo << "EMNetworkMessageSender::TaskEvent()" << ef;

	if(p_opTask -> taskId != m_vTaskID || m_opSession != NULL || p_opTask -> taskReqEvent != SONORK_CLIENT_REQUEST_EVENT_END)
		return;

	TSonorkServiceDataQueue* opQueue = EMNetworkEngine::Instance() -> GetAPI() -> GetTaskServices();

	if(opQueue != NULL)
	{
		TSonorkServiceData*	opService = opQueue -> RemoveFirst();

		opService -> physAddr.GetStr(vDummy);
		eo << "EMNetworkMessageSender::TaskEvent() - Got service: "  << vDummy << ef;
	
		m_opSession = EMNetworkEngine::Instance() -> GetAPI() -> CreateFileSession(opService, FileEvent, (DWORD)this);
		
		if(m_opSession != NULL)
		{
				eo << "EMNetworkMessageSender::TaskEvent() - Connecting" << ef;
				m_vPhase = EM_FILE_PHASE_CONNECTING;
		}
		else
		{
			eo << "EMNetworkMessageSender::TaskEvent() - Failed1" << ef;
			m_vPhase = EM_FILE_PHASE_ERROR;
		}
	}
	else
	{
		eo << "EMNetworkMessageSender::TaskEvent() - Failed2" << ef;
		m_vPhase = EM_FILE_PHASE_ERROR;
	}*/
}


int EMNetworkSendFileInfo::Upload()
{

	m_opSession = EMNetworkEngine::Instance() -> GetAPI() -> CreateFileSession(/*opService, */FileEvent, (DWORD)this);

//	m_vTaskID = EMNetworkEngine::Instance() -> GetAPI() -> ListServices(SONORK_SERVICE_TYPE_SONORK_FILE, 0, 0, 1, (DWORD)GetId());
	
//	m_vPhase = m_vTaskID != 0 ? EM_FILE_PHASE_RESOLVING : EM_FILE_PHASE_ERROR;
	m_vPhase = EM_FILE_PHASE_CONNECTING;

	return m_vTaskID != 0;
}
