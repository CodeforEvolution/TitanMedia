/*******************************************************
* Creator: Kaneda
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_TRANSITION_PROPERTIES
#define __MEDIA_INFO_COMMAND_GET_TRANSITION_PROPERTIES

#include "EMCommand.h"

class MediaInfoCommandGetTransitionProperties : public EMCommand
{
public:
	MediaInfoCommandGetTransitionProperties();
	void ExecuteE() {};
	void* ExecuteE(void* p_upClipID, void* p_upPropertyArray, void*);
	bool RequiresParameters();
	bool IsUndoable();
	void UndoE();
	EMCommand* CloneAndClear();

private:
};

#endif