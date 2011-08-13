/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_IMPORT_VIDEO
#define __MEDIA_COMMAND_IMPORT_VIDEO

#include "MediaCommandImportMedia.h"

class MediaCommandImportVideo : public MediaCommandImportMedia
{
public:
	MediaCommandImportVideo();
	void ExecuteE() {};
	void* ExecuteE(void* p_upFileName, void* p_upIDArray, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	int32 m_vReturnValue;
};

#endif
