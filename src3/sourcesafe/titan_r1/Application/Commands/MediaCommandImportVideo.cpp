#include "MediaCommandImportVideo.h"

#include "EMMediaEngine.h"
//#include "EMMediaProject.h"
//#include "EMBeFileInputDescriptor.h"

MediaCommandImportVideo::MediaCommandImportVideo()
{
}

void* MediaCommandImportVideo::ExecuteE(void* p_upFileName, void* p_upIDArray, void*)
{
	//TODO: Throw exception if fails:
	string oFileName(static_cast<const char*>(p_upFileName));
	int32* vpArray = static_cast<int32*>(p_upIDArray);
//	ExecuteImportMedia(oFileName, vpArray);
	return NULL;
}

bool MediaCommandImportVideo::RequiresParameters()
{
	return true;
}

bool MediaCommandImportVideo::IsUndoable()
{
	return false;
}