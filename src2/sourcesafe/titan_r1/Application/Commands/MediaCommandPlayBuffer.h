/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_PLAY_BUFFER
#define __MEDIA_COMMAND_PLAY_BUFFER

#include "EMCommand.h"

class MediaCommandPlayBuffer : public EMCommand
{
public:
	MediaCommandPlayBuffer();
	void ExecuteE();
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree) { return NULL; };
	bool RequiresParameters();
	bool IsUndoable();
	
private:

};

#endif
