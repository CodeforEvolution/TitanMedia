/*******************************************************
* Creator: Jesper Svensson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SET_RENDERING_FLAG
#define __MEDIA_COMMAND_SET_RENDERING_FLAG

#include "EMCommand.h"

class MediaCommandSetRenderingFlags : public EMCommand
{
public:
	MediaCommandSetRenderingFlags();
	void ExecuteE() {};
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
	bool RequiresParameters();
	bool IsUndoable();

private:

};

#endif
