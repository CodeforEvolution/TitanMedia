/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_ENTRY_USAGE_COUNT
#define __MEDIA_INFO_COMMAND_GET_ENTRY_USAGE_COUNT

#include "EMCommand.h"

class MediaInfoCommandGetEntryUsageCount : public EMCommand
{
public:
	MediaInfoCommandGetEntryUsageCount();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaPoolEntryID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	int64 m_vReturnValue;

};

#endif
