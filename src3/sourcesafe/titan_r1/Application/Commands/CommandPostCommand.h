/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* Used as a simple mailbox
* Uses the GUI service thread
* This implementation is verly simple and not thread safe
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_POST_COMMAND
#define __COMMAND_POST_COMMAND

#include "EMCommand.h"
#include "EMThreadListener.h"

class CommandPostCommand : public EMCommand, public EMThreadListener
{
public:
	CommandPostCommand();
	void ExecuteE() {};
	void* ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree);
	bool IsUndoable();
	bool RequiresParameters();
	void ThreadRun(EMThread* p_opThread);

private:
	uint32 m_vCommandID;
};

#endif
