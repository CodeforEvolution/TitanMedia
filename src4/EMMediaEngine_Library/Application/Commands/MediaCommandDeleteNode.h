/*******************************************************
* Creator: Jesper Svensson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_DELETE_NODE
#define __MEDIA_COMMAND_DELETE_NODE

#include "EMCommand.h"

class MediaCommandDeleteNode : public EMCommand
{
public:
	
	//Used for command instantiation
	MediaCommandDeleteNode();

	void* ExecuteE(void* p_upEMNodePtr, void* p_upTrackID, void* p_upParamID);
	bool RequiresParameters();
	bool IsUndoable();
	void UndoE();

private:

};

#endif
