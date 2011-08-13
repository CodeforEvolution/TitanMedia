/*******************************************************
* Creator: Kaneda
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_TRANSITION_NAME
#define __MEDIA_INFO_COMMAND_GET_TRANSITION_NAME

#include "EMCommand.h"

class MediaInfoCommandGetTransitionName : public EMCommand
{
public:
	MediaInfoCommandGetTransitionName();
	void ExecuteE();
	void* ExecuteE(void* p_upIDs, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();
private:
	string m_vpName;
};

#endif