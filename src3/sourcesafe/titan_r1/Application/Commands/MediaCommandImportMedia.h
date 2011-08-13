/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_IMPORT_MEDIA
#define __MEDIA_COMMAND_IMPORT_MEDIA

#include "EMCommand.h"

class MediaCommandImportMedia : public EMCommand
{
public:
	MediaCommandImportMedia();
	void ExecuteE() {};
	void* ExecuteE(void* p_upFileName, void* p_upArray, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:

};

#endif
