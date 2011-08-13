/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_TOGGLE_PLAYBACK
#define __MEDIA_COMMAND_TOGGLE_PLAYBACK

#include "EMCommand.h"

class MediaCommandTogglePlayback : public EMCommand
{
public:
	MediaCommandTogglePlayback();
	void ExecuteE();
	void* ExecuteE(void*, void*, void*) { return NULL; }
	bool RequiresParameters();
	bool IsUndoable();

private:
};

#endif
