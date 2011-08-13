#include "EMNetworkRecvFileInfo.h"
#include "EMNetworkEngine.h"
#include "EMNetworkUserDatabase.h"

#include "EMMessages.h"

#include "sonorkvcapi.h"


EMNetworkRecvFileInfo::EMNetworkRecvFileInfo(TSonorkMsg* p_opMsg)
  : m_opService(NULL)
{
	m_opInfo = new TSonorkFileInfo();

	if(m_opInfo -> CODEC_Read(&p_opMsg -> ExtData(), sonorkServerVersion) != SONORK_RESULT_OK)
	{
		delete m_opInfo;
		m_opInfo = NULL;
		m_vPhase = EM_FILE_PHASE_ERROR;
		return;
	}

	m_opRemoteUser = EMNetworkEngine::Instance() -> GetUserDatabase() -> Find(EMUserInfo::ConvertId(m_opInfo -> locus.creator));

	const char* opTmp =  m_opInfo -> name.CStr();

	if(*opTmp == '|')
	{
		m_vIsMediaMaterial = true;
		++opTmp;
	}

	m_oFileName = opTmp;
	m_vPhase = EM_FILE_PHASE_RESOLVING;

	eo << "EMNetworkRecvFileInfo - Added file: " << m_oFileName.c_str() << ef;
}


EMNetworkRecvFileInfo::~EMNetworkRecvFileInfo()
{
	eo << "EMNetworkRecvFileInfo::~EMNetworkRecvFileInfo() - " << m_oFilePath.c_str() << ef;

	if(m_opService != NULL)
	{
		delete m_opService;
	}
}

void EMNetworkRecvFileInfo::TaskEvent(TSonorkApiFileSession* p_opSession)
{
/*	EMNetworkRecvFileInfo* opRecv = reinterpret_cast<EMNetworkRecvFileInfo*>(p_opSession -> SessionTag());

	switch(p_opSession -> ApiEvent())
	{
	case SONORK_API_FILE_EVENT_PHASE_CHANGE:
		opRecv -> PhaseChange();
		break;
	case SONORK_API_FILE_EVENT_RECV_DATA:
		opRecv -> TransferData();
		break;
	}*/

	eo << "No way... get outta here!!" << ef;
}


int EMNetworkRecvFileInfo::Download()
{
	eo << "EMNetworkRecvFileInfo::Download()" << ef;

	m_hFile = CreateFile(m_oFilePath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if(m_hFile == INVALID_HANDLE_VALUE)
		return 0;

	m_vSize = GetFileSize(m_hFile, NULL);

	m_opSession = EMNetworkEngine::Instance() -> GetAPI() -> CreateFileSession(FileEvent, (DWORD)this);

	if(m_opSession == NULL)
	{
		m_vPhase = EM_FILE_PHASE_ERROR;
		return 0;
	}

	eo << "EMNetworkRecvFileInfo::Download() - Done" << ef;

	return 2;
}

void EMNetworkRecvFileInfo::PhaseChange()
{
	eo << "EMNetworkRecvFileInfo::PhaseChange()" << ef;

	switch(m_opSession -> TxPhase())
	{
	case SONORK_API_FILE_PHASE_ERROR:
		eo << "EMNetworkRecvFileInfo::PhaseChange() SONORK_API_FILE_PHASE_ERROR" << ef;
		break;

	case SONORK_API_FILE_PHASE_IDLE:
		eo << "EMNetworkRecvFileInfo::PhaseChange() SONORK_API_FILE_PHASE_IDLE" << ef;
		break;

	case SONORK_API_FILE_PHASE_CONNECTING:
		eo << "EMNetworkRecvFileInfo::PhaseChange() SONORK_API_FILE_PHASE_CONNECTING" << ef;
		break;

	case SONORK_API_FILE_PHASE_AUTHORIZING:
		eo << "EMNetworkRecvFileInfo::PhaseChange() SONORK_API_FILE_PHASE_AUTHORIZING" << ef;
		break;

	case SONORK_API_FILE_PHASE_CONNECTED:
		eo << "EMNetworkRecvFileInfo::PhaseChange() SONORK_API_FILE_PHASE_CONNECTED" << ef;

		if(m_opSession -> RequestGetFile(&(m_opInfo -> locus), false) != SONORK_RESULT_OK)
		{
			eo << "EMNetworkRecvFileInfo::PhaseChange() - Connected / Error" << ef;
			m_vPhase = EM_FILE_PHASE_ERROR;
		}
		break;

	case SONORK_API_FILE_PHASE_REQUESTING:
		eo << "EMNetworkRecvFileInfo::PhaseChange() SONORK_API_FILE_PHASE_REQUESTING" << ef;
		break;

	case SONORK_API_FILE_PHASE_TRANSMITTING:
		eo << "EMNetworkRecvFileInfo::PhaseChange() SONORK_API_FILE_PHASE_TRANSMITTING" << ef;
		break;

	case SONORK_API_FILE_PHASE_WAIT_CONFIRM:
		eo << "EMNetworkRecvFileInfo::PhaseChange() SONORK_API_FILE_PHASE_WAIT_CONFIRM" << ef;
		break;

	case SONORK_API_FILE_PHASE_COMPLETE:
		m_vPhase = EM_FILE_PHASE_DONE;

		eo << "EMNetworkRecvFileInfo::PhaseChange() SONORK_API_FILE_PHASE_COMPLETE" << ef;

		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
		
		EMNetworkEngine::Instance() -> Notify(EM_NETWORK_FILE_DONE);
		break;
	}

}


void EMNetworkRecvFileInfo::TransferData()
{
	DWORD vCnt;

	eo << "Receiving data for " << m_oFileName.c_str() << ef;

	if(!WriteFile(m_hFile, m_opSession -> DataBlockPtr(), m_opSession -> DataBlockSize(), &vCnt, NULL))
	{
		m_vPhase = EM_FILE_PHASE_ERROR;
	}

	m_vTransfered += vCnt;
}
