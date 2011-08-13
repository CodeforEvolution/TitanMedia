/*******************************************************
* Creator: Kaneda
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_NUMBER_OF_TRANSITION
#define __MEDIA_INFO_COMMAND_GET_NUMBER_OF_TRANSITION

#include "EMCommand.h"

class MediaInfoCommandGetNumberOfTransitions : public EMCommand
{
public:
	MediaInfoCommandGetNumberOfTransitions();
	void ExecuteE();
	void* ExecuteE(void*, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();
private:
	int32 m_vNumTrans;
};

#endif