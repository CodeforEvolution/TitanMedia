#include "EMNetworkFileHandler.h"
#include "EMNetworkEngine.h"
#include "EMNetworkUserDatabase.h"
#include "EMUserInfo.h"

#include <windows.h>

#include "sonorkvcapi.h"


EMNetworkFileHandler::EMNetworkFileHandler()
{
}

EMNetworkFileHandler::~EMNetworkFileHandler()
{
}

uint64 EMNetworkFileHandler::AddFile(TSonorkMsg* p_opMsg)
{
	EMNetworkRecvFileInfo* opInfo = new EMNetworkRecvFileInfo(p_opMsg);

	if(opInfo -> m_vPhase == EM_FILE_PHASE_ERROR)
	{
		delete opInfo;
		return -1;
	}

	Add(opInfo);

	return opInfo -> GetId();
}


EMNetworkRecvFileInfo* EMNetworkFileHandler::GetNextFile(EMFilePhase p_vStatus)
{
	EMNetworkRecvFileInfo* opInfo;

	LockContainer();
	Rewind();

	while(Current() != NULL && Current() -> m_vPhase != p_vStatus)
		Next();

	opInfo = Current();
	UnlockContainer();

	return opInfo;
}


void EMNetworkFileHandler::TimeSlot()
{
	LockContainer();
	Rewind();

	while(Current() != NULL)
	{
		if(Current() -> m_vPhase == EM_FILE_PHASE_ERROR)
		{
			eo << "EMNetworkFileHandler::TimeSlot - Removing file: " << Current() -> m_oFileName.c_str() << ef;

			EMNetworkRecvFileInfo* opInfo = Current();
			Remove(opInfo);
			delete opInfo;
			Rewind();
		}
		Next();
	}
	
	Rewind();
	while(Current() != NULL)
	{
		if(Current() -> m_opSession != NULL)
		{
			eo << "EMNetworkFileHandler::TimeSlot() - Ticking:" << Current() -> m_oFileName.c_str() << ef;
			Current() -> m_opSession -> TimeSlot();
		}
		Next();
	}
	UnlockContainer();
}