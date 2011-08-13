/*******************************************************
* This code is the copyrighted property of Elegant Media
*-------------------------------------------------------
* Creator: Johan Dunberg
* Component: Command
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/
#include "EMGlobals.h"

#ifndef __COMMAND_UPDATE_INSTANCE_IN_MEDIA_POOL_VIEW
#define __COMMAND_UPDATE_INSTANCE_IN_MEDIA_POOL_VIEW

#include "EMCommand.h"

class CommandUpdateInstanceInMediaPoolView : public EMCommand
{
public:
	CommandUpdateInstanceInMediaPoolView();
	void ExecuteE(){};
	void* ExecuteE(void* p_opFullpathFilename, void* , void* );
	bool IsUndoable();
	bool RequiresParameters();
};

#endif