/*******************************************************
* Creator: Jesper Svensson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_ADD_NODE
#define __MEDIA_COMMAND_ADD_NODE

#include "EMCommand.h"

class MediaCommandAddNode : public EMCommand
{
public:
	
	//Used for command instantiation
	MediaCommandAddNode();

	void* ExecuteE(void* p_upValues, void* p_upTrackID, void* p_upParamID);
	bool RequiresParameters();
	bool IsUndoable();
	void UndoE();

private:

};

#endif
