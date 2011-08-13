/*******************************************************
* Creator: Kaneda
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_DEFAULT_TRANSITION_PROPERTIES
#define __MEDIA_INFO_COMMAND_GET_DEFAULT_TRANSITION_PROPERTIES

#include "EMCommand.h"

class MediaInfoCommandGetDefaultTransitionProperties : public EMCommand
{
public:
	MediaInfoCommandGetDefaultTransitionProperties();
	void ExecuteE() {};
	void* ExecuteE(void* p_upTransID, void* p_upPropertyArray, void*);
	bool RequiresParameters();
	bool IsUndoable();
	void UndoE();
	EMCommand* CloneAndClear();

private:
};

#endif