/*******************************************************
* Creator: Kaneda
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_TRANSITION_IDS
#define __MEDIA_INFO_COMMAND_TRANSITION_IDS

#include "EMCommand.h"

class MediaInfoCommandGetTransitionIDs : public EMCommand
{
public:
	MediaInfoCommandGetTransitionIDs();
	void ExecuteE();
	void* ExecuteE(void* p_upIDs, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();
private:
	int32 m_vNumTrans;
};

#endif