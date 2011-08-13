/*******************************************************
* Creator: Kaneda
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SET_TRANSITION_PROPERTIES
#define __MEDIA_COMMAND_SET_TRANSITION_PROPERTIES

#include "EMCommand.h"

class MediaCommandSetTransitionProperties : public EMCommand
{
public:
	MediaCommandSetTransitionProperties();
	void ExecuteE() {};
	void* ExecuteE(void* p_upClipID, void* p_upPropertyArray, void*);
	bool RequiresParameters();
	bool IsUndoable();
	void UndoE();
	EMCommand* CloneAndClear();

private:
};

#endif