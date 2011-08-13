#include "MediaInfoCommandGetVideoEntryFrameRate.h"

#include "EMMediaClip.h"
#include "EMMediaEngine.h"
#include "EMMediaPool.h"
#include "EMMediaPoolEntry.h"
#include "EMMediaPoolVideoEntry.h"
#include "EMMediaProject.h"

MediaInfoCommandGetVideoEntryFrameRate::MediaInfoCommandGetVideoEntryFrameRate()
{
}

void* MediaInfoCommandGetVideoEntryFrameRate::ExecuteE(void* p_upPoolEntryID, void*, void*)
{
	//eo << "MediaInfoCommandGetVideoEntryFrameRate" << ef;
	int32 vPoolEntryID = *static_cast<int32*>(p_upPoolEntryID);
	EMMediaPool* opPool = EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool();
	EMMediaPoolEntry* opEntry = NULL;
	opPool -> LockContainer();
	try
	{
		opEntry = opPool -> Find(vPoolEntryID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetVideoEntryFrameRate::ExecuteE");
	}
	opPool -> UnlockContainer();
	if(opEntry == NULL)
		EMDebugger("ERROR! MediaInfoCommandGetAudioEntryNumSamples could not find entry id!");
//	if(! ((opEntry -> GetType() & EM_TYPE_ANY_VIDEO) > 0))
//		EMDebugger("ERROR! MediaInfoCommandGetVideoEntryFrameRate can only be called on VIDEO entries! This is obviously something else!");
	m_vReturnValue = static_cast<int32>(static_cast<EMMediaPoolVideoEntry*>(opEntry) -> GetFormat() -> m_vFrameRate);
	
	return static_cast<void*>(&m_vReturnValue);
}

bool MediaInfoCommandGetVideoEntryFrameRate::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetVideoEntryFrameRate::IsUndoable()
{
	return false;
}