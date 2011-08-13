#include "MediaInfoCommandGetVideoDimension.h"

#include "EMMediaEngine.h"
#include "EMMediaPool.h"
#include "EMMediaPoolEntry.h"
#include "EMMediaPoolVideoEntry.h"
#include "EMMediaProject.h"


MediaInfoCommandGetVideoDimension::MediaInfoCommandGetVideoDimension()
{
}

void* MediaInfoCommandGetVideoDimension::ExecuteE(void* p_upMediaPoolEntryID, void*, void*)
{
	int32 vMediaPoolEntryID = *static_cast<int32*>(p_upMediaPoolEntryID);

	EMMediaPool* opPool = EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool();
	
	opPool -> LockContainer();
	EMMediaPoolEntry* opEntry = opPool -> Find(vMediaPoolEntryID);
	opPool -> UnlockContainer();
	
	
	return static_cast<void*>((void*)static_cast<EMMediaPoolVideoEntry*>(opEntry) -> GetVideoDimension());
}

bool MediaInfoCommandGetVideoDimension::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetVideoDimension::IsUndoable()
{
	return false;
}