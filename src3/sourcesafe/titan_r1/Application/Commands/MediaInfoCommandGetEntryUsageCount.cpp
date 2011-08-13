#include "MediaInfoCommandGetEntryUsageCount.h"

#include "EMMediaEngine.h"
#include "EMMediaPool.h"
#include "EMMediaPoolEntry.h"
//#include "EMBeFileInputDescriptor.h"
#include "EMMediaProject.h"
#include "EMMediaClip.h"

MediaInfoCommandGetEntryUsageCount::MediaInfoCommandGetEntryUsageCount()
	:	m_vReturnValue(0)
{
}

void* MediaInfoCommandGetEntryUsageCount::ExecuteE(void* p_upMediaPoolEntryID, void*, void*)
{
	//eo << "MediaInfoCommandGetEntryUsageCount" << ef;
	//TODO: Throw exception if fails:
	int32 vMediaPoolEntryID = *(static_cast<int32*>(p_upMediaPoolEntryID));
	EMMediaPool* opPool = EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool();
	EMMediaPoolEntry* opEntry = NULL;
	opPool -> LockContainer();
	try
	{
		opEntry = opPool -> Find(vMediaPoolEntryID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetEntryUsageCount::ExecuteE");
	}
	opPool -> UnlockContainer();
	if(opEntry != NULL)
	{
		m_vReturnValue = (int64) opEntry -> GetUsage();
	}
	else
	{
//		char ch[80];
//		sprintf(ch, "ERROR! Could not find entry ID %d in media pool!", vMediaPoolEntryID);
//		opFileName = EM_new string(ch);
		m_vReturnValue = 0;
	}
		//
	
	//EMMediaPoolEntry* opEntry = opPool -> Find(vFileDescriptorID);
	
//	int64* vDuration = EM_new int64;
//	*vDuration = 0;
	
//	if(opDesc != NULL)
//		*vDuration = opEntry -> GetLength(); //microseconds
	
	return static_cast<void*>(&m_vReturnValue);
}

bool MediaInfoCommandGetEntryUsageCount::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetEntryUsageCount::IsUndoable()
{
	return false;
}