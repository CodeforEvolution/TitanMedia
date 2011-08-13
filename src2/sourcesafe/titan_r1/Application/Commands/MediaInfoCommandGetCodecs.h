/*******************************************************
* Creator: Jesper Svensson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_CODECS
#define __MEDIA_INFO_COMMAND_GET_CODECS

#include "EMCommand.h"

class MediaInfoCommandGetCodecs : public EMCommand
{
public:
	MediaInfoCommandGetCodecs();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaClipID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:

};

#endif
