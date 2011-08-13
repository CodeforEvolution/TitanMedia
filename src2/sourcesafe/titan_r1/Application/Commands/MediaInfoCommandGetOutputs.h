/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_OUTPUTS
#define __MEDIA_INFO_COMMAND_GET_OUTPUTS

#include "EMCommand.h"

class MediaInfoCommandGetOutputs : public EMCommand
{
public:
	MediaInfoCommandGetOutputs();
	~MediaInfoCommandGetOutputs();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaType, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	list<int32> m_oReturnValue;
};

#endif
