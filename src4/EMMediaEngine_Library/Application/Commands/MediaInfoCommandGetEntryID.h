/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_ENTRY_ID
#define __MEDIA_INFO_COMMAND_GET_ENTRY_ID

#include "EMCommand.h"

class MediaInfoCommandGetEntryID : public EMCommand
{
public:
	MediaInfoCommandGetEntryID();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaClipID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	int32 m_vReturnValue;

};

#endif
