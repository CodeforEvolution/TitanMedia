/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_INPUT_NAME
#define __MEDIA_INFO_COMMAND_GET_INPUT_NAME

#include "EMCommand.h"

class MediaInfoCommandGetInputName : public EMCommand
{
public:
	MediaInfoCommandGetInputName();
	void ExecuteE() {};
	void* ExecuteE(void* p_upInputID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	char m_vpReturnValue[1024];

};

#endif
