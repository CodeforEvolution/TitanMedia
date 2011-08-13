/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_ENTRY_INFO
#define __MEDIA_INFO_COMMAND_GET_ENTRY_INFO

#include "EMCommand.h"

class MediaInfoCommandGetEntryInfo : public EMCommand
{
public:
	MediaInfoCommandGetEntryInfo();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaClipID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	EMMediaPoolEntryInfo m_oReturnValue;

};

#endif
