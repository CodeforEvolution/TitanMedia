/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_IMPORT_AUDIO
#define __MEDIA_COMMAND_IMPORT_AUDIO

#include "MediaCommandImportMedia.h"

class MediaCommandImportAudio : public MediaCommandImportMedia
{
public:
	MediaCommandImportAudio();
	void ExecuteE() {};
	void* ExecuteE(void* p_upFileName, void* p_upIDArray, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	int32 m_vReturnValue;

};

#endif
