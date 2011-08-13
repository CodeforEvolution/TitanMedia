#include "MediaInfoCommandGetVideoAspectRatio.h"

#include "EMMediaEngine.h"
#include "EMMediaPool.h"
#include "EMMediaPoolEntry.h"
#include "EMMediaPoolVideoEntry.h"
#include "EMMediaPoolImageEntry.h"
//#include "EMBeFileInputDescriptor.h"
#include "EMMediaProject.h"
#include "EMMediaClip.h"
#include "EMMediaFormat.h"

#include <list>

MediaInfoCommandGetVideoAspectRatio::MediaInfoCommandGetVideoAspectRatio()
{
}

void* MediaInfoCommandGetVideoAspectRatio::ExecuteE(void* p_upMediaPoolEntryID, void*, void*)
{
	//cout << "MediaInfoCommandGetVideoAspectRatio" << endl;
	//TODO: Throw exception if fails:
	int32 vMediaPoolEntryID = *static_cast<int32*>(p_upMediaPoolEntryID);
	EMMediaPool* opPool = EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool();
	EMMediaPoolEntry* opEntry = NULL;
	opPool -> LockContainer();
	try
	{
		opEntry = opPool -> Find(vMediaPoolEntryID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetVideoAspectRatio::ExecuteE");
	}
	opPool -> UnlockContainer();

	if(opEntry != NULL)
	{
		if((opEntry -> GetType() & EM_TYPE_ANY_VIDEO) > 0)
		{
			return static_cast<void*>(static_cast<EMMediaPoolVideoEntry*>(opEntry) -> GetAspectRatio());
		}
		if((opEntry -> GetType() & EM_TYPE_ANY_IMAGE) > 0)
		{
			return static_cast<void*>(static_cast<EMMediaPoolImageEntry*>(opEntry) -> GetAspectRatio());
		}
		else
			EMDebugger("ERROR! This seems to be a NON video CLIP (Should be video .. FIXXXX (Martin?)");
	}
	else
		EMDebugger("ERROR! MediaInfoCommandGetVideoClipContent could not find pool entry ID");

	return NULL;
}

bool MediaInfoCommandGetVideoAspectRatio::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetVideoAspectRatio::IsUndoable()
{
	return false;
}