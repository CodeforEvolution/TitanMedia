/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_WRITE_STATUS_BAR_MESSAGE
#define __COMMAND_WRITE_STATUS_BAR_MESSAGE

#include "EMCommand.h"
#include "EMThreadListener.h"

const int MESSAGE_BUFFER_SIZE = 1000;

class CommandWriteStatusBarMessage : public EMCommand, public EMThreadListener
{
public:
	CommandWriteStatusBarMessage();
	void ExecuteE() {};
	void* ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree);
	bool IsUndoable();
	bool RequiresParameters();
	void ThreadRun(EMThread* p_opThread);

private:
	char m_vpMessageBuffer[MESSAGE_BUFFER_SIZE];
};

#endif
