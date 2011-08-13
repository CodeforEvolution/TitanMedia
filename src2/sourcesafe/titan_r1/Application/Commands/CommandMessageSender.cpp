#include "CommandMessageSender.h"

#include "EMListener.h"

CommandMessageSender::CommandMessageSender()
{
}

void* CommandMessageSender::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{
	uint32 vMessageID = *(static_cast<uint32*>(p_upParameterOne));
;//cout_commented_out_4_release << "CommandMessageSender::ExecuteE: vMessageID: " << vMessageID << endl;
	EMListener* vListener = static_cast<EMListener*>(p_upParameterTwo);
	EMListenerRepository* opListenerRepository;
	if(p_upParameterThree == NULL)
		opListenerRepository = this;
	else
		opListenerRepository = static_cast<EMListenerRepository*>(p_upParameterThree);
	vListener -> MessageReceived(opListenerRepository, vMessageID);
	return NULL;
}

bool CommandMessageSender::IsUndoable()
{
	return false;
}

bool CommandMessageSender::RequiresParameters()
{
	return true;
}


