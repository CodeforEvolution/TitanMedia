/*******************************************************
* Creator: Kaneda
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SET_TRANSITION
#define __MEDIA_COMMAND_SET_TRANSITION

#include "EMCommand.h"

class MediaCommandSetTransition : public EMCommand
{
public:
	MediaCommandSetTransition();
	MediaCommandSetTransition(int32 p_vClipID, int32 p_vOldTransID);
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaClipID, void* p_upTransID, void*);
	bool RequiresParameters();
	bool IsUndoable();
	void UndoE();
	EMCommand* CloneAndClear();

private:
	int32 m_vClipID;
	int32 m_vOldTransID;

};

#endif