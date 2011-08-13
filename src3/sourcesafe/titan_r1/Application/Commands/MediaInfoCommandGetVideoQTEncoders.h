/*******************************************************
* Creator: Jesper Svensson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

//#include "EMGlobals.h"
//#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_VIDEO_QT_ENCODERS
#define __MEDIA_INFO_COMMAND_GET_VIDEO_QT_ENCODERS

#include "EMCommand.h"

class MediaInfoCommandGetVideoQTEncoders : public EMCommand
{
public:
	MediaInfoCommandGetVideoQTEncoders();
	void ExecuteE() {};
	void* ExecuteE(void* p_upIDs, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:

};

#endif
