#include "NetworkInfoCommandGetNextDownloadedFile.h"

#include "EMNetworkEngine.h"
#include "EMNetworkFileHandler.h"
#include "EMNetworkRecvFileInfo.h"
#include "EMUserInfo.h"

NetworkInfoCommandGetNextDownloadedFile::NetworkInfoCommandGetNextDownloadedFile()
: m_vpName(NULL)
{
}

NetworkInfoCommandGetNextDownloadedFile::~NetworkInfoCommandGetNextDownloadedFile()
{
	if(m_vpName != NULL)
		delete m_vpName;
}

void* NetworkInfoCommandGetNextDownloadedFile::ExecuteE(void* p_oppName, void* p_opToMediaPool, void*)
{
	const char** vppName = static_cast<const char**>(p_oppName);
	bool* vpToPool       = static_cast<bool*>(p_opToMediaPool);

	EMNetworkRecvFileInfo* opInfo = EMNetworkEngine::Instance() -> GetFileHandler() -> GetNextFile(EM_FILE_PHASE_DONE);

	if(opInfo != NULL)
	{
		opInfo -> m_vPhase = EM_FILE_PHASE_ERROR;

		m_vUID = opInfo -> m_opRemoteUser -> GetId();

		if(vppName != NULL)
			*vppName = opInfo -> m_oFilePath.c_str();

		if(vpToPool != NULL)
			*vpToPool = opInfo -> m_vIsMediaMaterial;
	}
	else
	{
		m_vUID = -1;
	}

	return &m_vUID;
}

bool NetworkInfoCommandGetNextDownloadedFile::IsUndoable()
{
	return false;
}

bool NetworkInfoCommandGetNextDownloadedFile::RequiresParameters()
{
	return true;
}