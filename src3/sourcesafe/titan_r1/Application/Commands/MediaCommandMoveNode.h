/*******************************************************
* Creator: Jesper Svensson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_MOVE_NODE
#define __MEDIA_COMMAND_MOVE_NODE

#include "EMCommand.h"

class MediaCommandMoveNode : public EMCommand
{
public:
	
	//Used for command instantiation
	MediaCommandMoveNode();

	void* ExecuteE(void* p_upEMNodePtr, void* p_upValues, void* p_upTrackID);
	bool RequiresParameters();
	bool IsUndoable();
	void UndoE();

private:

};

#endif
