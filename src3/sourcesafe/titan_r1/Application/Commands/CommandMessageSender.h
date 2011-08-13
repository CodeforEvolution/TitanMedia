/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* This class can be used to send standard uint32 messages
* to any EMListener
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_MESSAGE_SENDER
#define __COMMAND_MESSAGE_SENDER

#include "EMCommand.h"
#include "EMListenerRepository.h"

class CommandMessageSender : public EMCommand, public EMListenerRepository
{
public:
	CommandMessageSender();
	void ExecuteE() {};
	void* ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree);
	bool IsUndoable();
	bool RequiresParameters();
};

#endif
