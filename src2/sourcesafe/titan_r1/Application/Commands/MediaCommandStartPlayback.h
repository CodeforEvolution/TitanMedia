/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_START_PLAYBACK
#define __MEDIA_COMMAND_START_PLAYBACK

#include "EMCommand.h"

class MediaCommandStartPlayback : public EMCommand
{
public:
	MediaCommandStartPlayback();
	void ExecuteE();
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree) { return NULL; };
	bool RequiresParameters();
	bool IsUndoable();
	
private:

};

#endif
