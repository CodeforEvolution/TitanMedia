#include "MediaCommandImportMedia.h"

//#include "EMBeFileInputDescriptor.h"
#include "EMMediaEngine.h"
#include "EMMediaFileRepository.h"
#include "EMMediaPool.h"
#include "EMMediaProject.h"

#include "EMException.h"
#include "EMExceptionHandler.h"

MediaCommandImportMedia::MediaCommandImportMedia()
{
}

void* MediaCommandImportMedia::ExecuteE(void* p_upFileName, void* p_upArray, void*)
{
	//;//cout_commented_out_4_release << "MediaCommandImportMedia" << endl;
	string oFileName(static_cast<const char*>(p_upFileName));
	int32* vpArray = static_cast<int32*>(p_upArray);

	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	if(opEngine -> GetMediaProject() -> GetMediaPool() == NULL)
		EMDebugger("ERROR! Media pool is NULL!");
	try
	{
		EMMediaPool* opPool = opEngine -> GetMediaProject() -> GetMediaPool();
		opPool -> AddMediaFileE(oFileName, vpArray);
		EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
	}
	catch(EMException* e)
	{
		EMExceptionHandler::Instance() -> HandleException(*e);
	}
	return NULL;
}

bool MediaCommandImportMedia::RequiresParameters()
{
	return true;
}

bool MediaCommandImportMedia::IsUndoable()
{
	return false;
}
