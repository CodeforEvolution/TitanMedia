#include "CommandPostCommand.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGUIGlobals.h"
#include "EMThread.h"
#include "EMThreadRepository.h"

CommandPostCommand::CommandPostCommand()
{
}

void* CommandPostCommand::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{
	m_vCommandID = *(static_cast<uint32*>(p_upParameterOne));
	EMThreadRepository::Instance() -> GetThread(GUI_SERVICE_THREAD_NAME) -> AddListener(this);
	return NULL;
}

bool CommandPostCommand::IsUndoable()
{
	return false;
}

bool CommandPostCommand::RequiresParameters()
{
	return true;
}


void CommandPostCommand::ThreadRun(EMThread* p_opThread)
{
	EMCommandRepository::Instance() -> ExecuteCommand(m_vCommandID);
	EMThreadRepository::Instance() -> GetThread(GUI_SERVICE_THREAD_NAME) -> RemoveListener(this);
}