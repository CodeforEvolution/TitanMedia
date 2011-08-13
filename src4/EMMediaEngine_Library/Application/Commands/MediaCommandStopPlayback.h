/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_STOP_PLAYBACK
#define __MEDIA_COMMAND_STOP_PLAYBACK

#include "EMCommand.h"

class MediaCommandStopPlayback : public EMCommand
{
public:
	MediaCommandStopPlayback();
	void ExecuteE();
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree) { return NULL; };
	bool RequiresParameters();
	bool IsUndoable();

private:

};

#endif
