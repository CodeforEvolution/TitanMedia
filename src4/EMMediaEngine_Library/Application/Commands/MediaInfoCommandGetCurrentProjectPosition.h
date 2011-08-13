/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_CURRENT_PROJECT_POSITION
#define __MEDIA_INFO_COMMAND_GET_CURRENT_PROJECT_POSITION

#include "EMCommand.h"

class MediaInfoCommandGetCurrentProjectPosition : public EMCommand
{
public:
	MediaInfoCommandGetCurrentProjectPosition();
	void ExecuteE() {};
	void* ExecuteE(void*, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	int64 m_vReturnValue;
};

#endif
