/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_ENTRY_FILE_NAME
#define __MEDIA_INFO_COMMAND_GET_ENTRY_FILE_NAME

#include "EMCommand.h"

//Takes an entry ID and returns a CONST CHAR*

class MediaInfoCommandGetEntryFileName : public EMCommand
{
public:
	MediaInfoCommandGetEntryFileName();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaPoolEntryID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	char m_vpReturnValue[1024];

};

#endif
