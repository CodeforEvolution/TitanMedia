#include "NetworkInfoCommandGetNextFileInfo.h"

#include "EMNetworkEngine.h"
#include "EMNetworkFileHandler.h"
#include "EMNetworkRecvFileInfo.h"
#include "EMUserInfo.h"

NetworkInfoCommandGetNextFileInfo::NetworkInfoCommandGetNextFileInfo()
{
}

NetworkInfoCommandGetNextFileInfo::~NetworkInfoCommandGetNextFileInfo()
{
}

void* NetworkInfoCommandGetNextFileInfo::ExecuteE(void* p_opFileID, void* p_oppName, void* p_opToMediaPool)
{
	uint32* vpFileID     = static_cast<uint32*>(p_opFileID);
	const char** vppName = static_cast<const char**>(p_oppName);
	bool* vpToPool       = static_cast<bool*>(p_opToMediaPool);

	EMNetworkRecvFileInfo* opInfo = EMNetworkEngine::Instance() -> GetFileHandler() -> GetNextFile(EM_FILE_PHASE_RESOLVING);

	if(opInfo != NULL)
	{
		opInfo -> m_vPhase = EM_FILE_PHASE_CONNECTING;

		m_vUID = opInfo -> m_opRemoteUser -> GetId();

		if(vppName != NULL)
			*vppName = opInfo -> m_oFileName.c_str();

		if(vpFileID != NULL)
			*vpFileID = opInfo -> GetId();

		if(vpToPool != NULL)
			*vpToPool = opInfo -> m_vIsMediaMaterial;
	}
	else
	{
		m_vUID = -1;
	}

	return &m_vUID;
}

bool NetworkInfoCommandGetNextFileInfo::IsUndoable()
{
	return false;
}

bool NetworkInfoCommandGetNextFileInfo::RequiresParameters()
{
	return true;
}