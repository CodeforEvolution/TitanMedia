#include "MediaCommandImportAudio.h"

#include "EMMediaEngine.h"
//#include "EMMediaProject.h"

MediaCommandImportAudio::MediaCommandImportAudio()
{
}

void* MediaCommandImportAudio::ExecuteE(void* p_upFileName, void* p_upIDArray, void*)
{
	//TODO: Throw exception if fails:
	string oFileName(static_cast<const char*>(p_upFileName));
	int32* vpArray = static_cast<int32*>(p_upIDArray);
//	ExecuteImportMedia(oFileName, vpArray);
	return NULL;
}

bool MediaCommandImportAudio::RequiresParameters()
{
	return true;
}

bool MediaCommandImportAudio::IsUndoable()
{
	return false;
}