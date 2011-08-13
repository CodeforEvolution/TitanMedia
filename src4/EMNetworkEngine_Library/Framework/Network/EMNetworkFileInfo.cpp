#include "EMNetworkFileInfo.h"

#include "sonorkvcapi.h"

uint64 EMNetworkFileInfo::m_vNextID = 0;

EMNetworkFileInfo::EMNetworkFileInfo()
 :  m_vID(m_vNextID++),
	m_vSending(false),
	m_opSession(NULL),
	m_vTaskID(-1),
	m_opRemoteUser(NULL),
	m_vTransfered(-1),
	m_vSize(-1),
	m_vPhase(EM_FILE_PHASE_NONE),
	m_oFilePath(""),
	m_oFileName(""),
	m_hFile(INVALID_HANDLE_VALUE),
	m_vIsMediaMaterial(false)

{
}


EMNetworkFileInfo::~EMNetworkFileInfo()
{
	eo << "EMNetworkFileInfo::~EMNetworkFileInfo() - " << m_oFilePath.c_str() <<"\\" << m_oFileName.c_str() << ef;

	if(m_hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}

	if(m_opSession != NULL)
	{
		m_opSession -> DecRefCount();
		m_opSession = NULL;
	}
}

uint64 EMNetworkFileInfo::GetId()
{
	return m_vID;
}

void EMNetworkFileInfo::FileEvent(TSonorkApiFileSession* p_opSession)
{
	eo << "EMNetworkFileInfo::FileEvent - Begin" << ef;

	EMNetworkFileInfo* opInfo = reinterpret_cast<EMNetworkFileInfo*>(p_opSession -> SessionTag());

	switch(p_opSession -> ApiEvent())
	{
	case SONORK_API_FILE_EVENT_PHASE_CHANGE:
		eo << "EMNetworkFileInfo::FileEvent - SONORK_API_FILE_EVENT_PHASE_CHANGE" << ef;
		opInfo -> PhaseChange();
		break;
	case SONORK_API_FILE_EVENT_SEND_DATA:
	case SONORK_API_FILE_EVENT_RECV_DATA:
		eo << "EMNetworkFileInfo::FileEvent - SONORK_API_FILE_EVENT_RECV_DATA" << ef;

		opInfo -> TransferData();
		break;
	}
}