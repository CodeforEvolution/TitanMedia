/*******************************************************
* Creator: Jesper Svensson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_NODES
#define __MEDIA_INFO_COMMAND_GET_NODES

#include "EMCommand.h"

class MediaInfoCommandGetNodes : public EMCommand
{
public:
	
	//Used for command instantiation
	MediaInfoCommandGetNodes();

	void* ExecuteE(void* p_upTrackID, void* p_upParamID, void* p_upParameterThree);
	bool RequiresParameters();
	bool IsUndoable();
	void UndoE();

private:

};

#endif
