/*******************************************************
* Creator: Jesper Svensson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_HAS_QUALITY
#define __MEDIA_INFO_COMMAND_HAS_QUALITY

#include "EMCommand.h"

class MediaInfoCommandHasQuality : public EMCommand
{
public:
	MediaInfoCommandHasQuality();
	void ExecuteE() {};
	void* ExecuteE(void* p_vEncoderID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	bool vHasQuality;
};

#endif
