/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_PREPARE_RECORDING
#define __MEDIA_COMMAND_PREPARE_RECORDING

#include "EMCommand.h"

class MediaCommandPrepareRecording : public EMCommand
{
public:
	MediaCommandPrepareRecording();
	void ExecuteE();
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree) { return NULL; };
	bool RequiresParameters();
	bool IsUndoable();
	
private:

};

#endif
