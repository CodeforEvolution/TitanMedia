/*******************************************************
* Creator: Jesper Svensson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SET_RENDER_TO_DISK_OUTPUT_FILENAME
#define __MEDIA_COMMAND_SET_RENDER_TO_DISK_OUTPUT_FILENAME

#include "EMCommand.h"

class MediaCommandSetRenderToDiskOutputFileName : public EMCommand
{
public:
	MediaCommandSetRenderToDiskOutputFileName();
	void ExecuteE() {};
	void* ExecuteE(void* p_vName, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:

};

#endif
