/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_OUTPUT_NAME
#define __MEDIA_INFO_COMMAND_GET_OUTPUT_NAME

#include "EMCommand.h"

//Takes an output ID and returns a CONST CHAR*

class MediaInfoCommandGetOutputName : public EMCommand
{
public:
	MediaInfoCommandGetOutputName();
	void ExecuteE() {};
	void* ExecuteE(void* p_upOutputID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	char m_vpReturnValue[1024];
};

#endif
