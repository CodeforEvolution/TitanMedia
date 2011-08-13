/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_ENTRY_DURATION
#define __MEDIA_INFO_COMMAND_GET_ENTRY_DURATION

#include "EMCommand.h"

class MediaInfoCommandGetEntryDuration : public EMCommand
{
public:
	MediaInfoCommandGetEntryDuration();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaEntryID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	int64 m_vReturnValue;

};

#endif
