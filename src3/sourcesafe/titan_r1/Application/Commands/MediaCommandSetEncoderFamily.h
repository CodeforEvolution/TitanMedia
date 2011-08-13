/*******************************************************
* Creator: Jesper Svensson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SET_ENCODER_FAMILY
#define __MEDIA_COMMAND_SET_ENCODER_FAMILY

#include "EMCommand.h"

class MediaCommandSetEncoderFamily : public EMCommand
{
public:
	MediaCommandSetEncoderFamily();
	void ExecuteE() {};
	void* ExecuteE(void* p_vEncoderID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:

};

#endif
