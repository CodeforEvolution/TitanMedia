/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_INPUTS
#define __MEDIA_INFO_COMMAND_GET_INPUTS

#include "EMCommand.h"

class MediaInfoCommandGetInputs : public EMCommand
{
public:
	MediaInfoCommandGetInputs();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaType, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	list<int32> m_oReturnValue;
};

#endif
