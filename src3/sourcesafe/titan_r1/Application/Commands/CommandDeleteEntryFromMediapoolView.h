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

#ifndef __COMMAND_DELETE_ENTRY_FROM_MEDIA_POOL_VIEW
#define __COMMAND_DELETE_ENTRY_FROM_MEDIA_POOL_VIEW

#include "EMCommand.h"

class CommandDeleteEntryFromMediaPoolView : public EMCommand
{
public:
	CommandDeleteEntryFromMediaPoolView();
	void ExecuteE(){};
	void* ExecuteE(void* p_opFullpathFilename, void* , void* );
	bool IsUndoable();
	bool RequiresParameters();
};

#endif