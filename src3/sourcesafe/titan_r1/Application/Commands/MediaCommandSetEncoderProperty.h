/*******************************************************
* Creator: Jesper Svensson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SET_ENCODER_PROPERTY
#define __MEDIA_COMMAND_SET_ENCODER_PROPERTY

#include "EMCommand.h"

class MediaCommandSetEncoderProperty : public EMCommand
{
public:
	MediaCommandSetEncoderProperty();
	void ExecuteE() {};
	void* ExecuteE(void* p_vEncoderID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:

};

#endif
