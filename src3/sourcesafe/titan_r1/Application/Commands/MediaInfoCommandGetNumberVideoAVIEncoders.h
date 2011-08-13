/*******************************************************
* Creator: Jesper Svensson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_NUMBER_VIDEO_AVI_ENCODERS
#define __MEDIA_INFO_COMMAND_GET_NUMBER_VIDEO_AVI_ENCODERS

#include "EMCommand.h"

class MediaInfoCommandGetNumberVideoAVIEncoders : public EMCommand
{
public:
	MediaInfoCommandGetNumberVideoAVIEncoders();
	void ExecuteE() {};
	void* ExecuteE(void*, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
 int32 m_vNumVideoEnc;
};

#endif
