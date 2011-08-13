/*******************************************************
* Creator: Jesper Svensson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_ENCODER_PROPERTIES
#define __MEDIA_INFO_COMMAND_GET_ENCODER_PROPERTIES

#include "EMCommand.h"

class MediaInfoCommandGetEncoderProperties : public EMCommand
{
public:
	MediaInfoCommandGetEncoderProperties();
	void ExecuteE() {};
	void* ExecuteE(void* p_vEncoderID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	char* m_vEncoderPropertyPointer;
};

#endif
