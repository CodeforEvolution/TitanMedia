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

#ifndef __COMMAND_GET_ENTRIES_FROM_MEDIA_POOL_VIEW
#define __COMMAND_GET_ENTRIES_FROM_MEDIA_POOL_VIEW

#include "EMCommand.h"

class EMView;

class CommandGetEntriesFromMediaPoolView : public EMCommand
{
public:
	CommandGetEntriesFromMediaPoolView();
	void ExecuteE(){};
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
	bool IsUndoable();
	bool RequiresParameters();

private:
	EMView* m_opMediaPoolView;
	bool m_vNewEntry;
};

#endif
