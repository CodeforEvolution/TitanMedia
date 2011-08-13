/*******************************************************
* Creator: Jesper Svensson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_ENCODER_NAME
#define __MEDIA_INFO_COMMAND_GET_ENCODER_NAME

#include "EMCommand.h"

class MediaInfoCommandGetEncoderName : public EMCommand
{
public:
	MediaInfoCommandGetEncoderName();
	void ExecuteE() {};
	void* ExecuteE(void* p_upIDs, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	string m_vpName;
};

#endif
