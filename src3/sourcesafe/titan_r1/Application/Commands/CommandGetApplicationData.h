/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* Contains some native data
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_GET_APPLICATION_DATA
#define __COMMAND_GET_APPLICATION_DATA

#include "EMCommand.h"

// p_upParameterOne (uint32) determines what application data is returned:
// 1 : The native main window
// 2: The application instance (if it is relevant for the particular platform)

class CommandGetApplicationData : public EMCommand
{
public:
	CommandGetApplicationData();
	void ExecuteE() {};
	void* ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree);
	bool IsUndoable();
	bool RequiresParameters();
};

#endif
