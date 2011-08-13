/*******************************************************
* Creator: Kaneda
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_TRANSITION
#define __MEDIA_INFO_COMMAND_GET_TRANSITION

#include "EMCommand.h"

class MediaInfoCommandGetTransition : public EMCommand
{
public:
	MediaInfoCommandGetTransition();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaClipID, void* p_upTransID, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	int32 m_vTransID;
};

#endif