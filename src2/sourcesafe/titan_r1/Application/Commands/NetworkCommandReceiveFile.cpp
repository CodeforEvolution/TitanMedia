#include "NetworkCommandReceiveFile.h"
#include "EMNetworkEngine.h"
#include "EMNetworkFileHandler.h"
#include "EMNetworkRecvFileInfo.h"

NetworkCommandReceiveFile::NetworkCommandReceiveFile()
{
}

NetworkCommandReceiveFile::~NetworkCommandReceiveFile()
{
}

void* NetworkCommandReceiveFile::ExecuteE(void* p_opID, void* p_opFileName, void* p_opParameterThree)
{
	uint32  vID = *(static_cast<uint32*>(p_opID));
	char*   vpName = static_cast<char*>(p_opFileName);

	eo << "NetworkCommandReceiveFile::ExecuteE(" << vpName<<");" << ef;

	EMNetworkRecvFileInfo* opInfo = EMNetworkEngine::Instance() -> GetFileHandler() -> Find(vID);

	if(opInfo != NULL)
	{
		eo << "a'ok" << ef;
		opInfo -> m_oFilePath = vpName;
		m_vStatus = opInfo -> Download();
	}
	else
	{
		eo << "a'error" << ef;
		m_vStatus = 0;
	}

	return &m_vStatus;
}

bool NetworkCommandReceiveFile::IsUndoable()
{
	return false;
}

bool NetworkCommandReceiveFile::RequiresParameters()
{
	return true;
}