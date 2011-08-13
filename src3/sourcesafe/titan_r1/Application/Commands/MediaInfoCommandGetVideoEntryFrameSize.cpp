#include "MediaInfoCommandGetVideoEntryFrameSize.h"

#include "EMMediaClip.h"
#include "EMMediaEngine.h"
#include "EMMediaPool.h"
#include "EMMediaPoolEntry.h"
#include "EMMediaPoolVideoEntry.h"
#include "EMMediaProject.h"

MediaInfoCommandGetVideoEntryFrameSize::MediaInfoCommandGetVideoEntryFrameSize()
{
}

void* MediaInfoCommandGetVideoEntryFrameSize::ExecuteE(void* p_upPoolEntryID, void* p_upXSizePointer, void* p_upYSizePointer)
{
	//eo << "MediaInfoCommandGetVideoEntryFrameSize" << ef;
	int32 vPoolEntryID = *static_cast<int32*>(p_upPoolEntryID);
	uint64* vpXSize = static_cast<uint64*>(p_upXSizePointer);
	uint64* vpYSize = static_cast<uint64*>(p_upYSizePointer);

	ASSERT(vpXSize != NULL && vpYSize != NULL);

	EMMediaPool* opPool = EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool();
	EMMediaPoolEntry* opEntry = NULL;
	opPool -> LockContainer();
	try
	{
		opEntry = opPool -> Find(vPoolEntryID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetVideoEntryFrameSize::ExecuteE");
	}
	opPool -> UnlockContainer();

	try
	{
		if(opEntry == NULL)
			EMDebugger("ERROR! MediaInfoCommandGetVideoEntryFrameSize could not find entry id!");
		if((opEntry -> GetType() & EM_TYPE_ANY_VIDEO) == 0)
			EMDebugger("ERROR! MediaInfoCommandGetVideoEntryFrameSize can only be called on VIDEO entries! This is obviously something else!");
		if(opEntry -> GetFormat() == NULL)
			EMDebugger("ERROR! No format in video entry!");



		(*vpXSize) = static_cast<uint64>(static_cast<EMMediaPoolVideoEntry*>(opEntry) -> GetFormat() -> m_vWidth);
		(*vpYSize) = static_cast<uint64>(static_cast<EMMediaPoolVideoEntry*>(opEntry) -> GetFormat() -> m_vHeight);
	}
	catch(...)
	{
		EMDebugger("ERROR! Caught an exception while fetching video format size(s)");
	}
	
	return static_cast<void*>(NULL);
}

bool MediaInfoCommandGetVideoEntryFrameSize::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetVideoEntryFrameSize::IsUndoable()
{
	return false;
}