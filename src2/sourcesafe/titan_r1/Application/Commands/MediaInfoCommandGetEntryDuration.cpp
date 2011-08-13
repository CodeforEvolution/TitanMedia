#include "MediaInfoCommandGetEntryDuration.h"

#include "EMMediaEngine.h"
#include "EMMediaPool.h"
#include "EMMediaPoolEntry.h"
#include "EMFileInputDescriptor.h"
#include "EMMediaProject.h"
#include "EMMediaClipRepository.h"
#include "EMMediaClip.h"

MediaInfoCommandGetEntryDuration::MediaInfoCommandGetEntryDuration()
{
}

void* MediaInfoCommandGetEntryDuration::ExecuteE(void* p_upMediaEntryID, void*, void*)
{
	//eo << "MediaInfoCommandGetEntryDuration" << ef;
	//TODO: Throw exception if fails:
	int32 vMediaEntryID = *(static_cast<int32*>(p_upMediaEntryID));
	EMMediaPool* opPool = EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool();

//	EMMediaClipRepository* opAudioClipRepository = EMMediaEngine::Instance() -> GetMediaProject() -> GetAudioClipRepository();
//	EMMediaClipRepository* opVideoClipRepository = EMMediaEngine::Instance() -> GetMediaProject() -> GetVideoClipRepository();
//	EMMediaClip* opClip = NULL;	
//	opAudioClipRepository -> LockContainer();
//	try
//	{
//		opClip = opAudioClipRepository -> Find(vMediaClipID);
//	}
//	catch(...)
//	{
//		EMDebugger("ERROR! Exception in MediaInfoCommandGetEntryDuration::ExecuteE");
//	}
//	opAudioClipRepository -> UnlockContainer();
//	if(opClip == NULL)
//	{
//		opVideoClipRepository -> LockContainer();
//		try
//		{
//			opClip = opVideoClipRepository -> Find(vMediaClipID);
//		}
//		catch(...)
//		{
//			EMDebugger("ERROR! Exception in MediaInfoCommandGetEntryDuration::ExecuteE");
//		}
//		opVideoClipRepository -> UnlockContainer();
//	}
//	if(opClip == NULL)
//		EMDebugger("ERROR! MediaInfoCommandGetEntryDuration could not find clip!");
	
//	EMFileInputDescriptor* opDesc = opClip -> GetDescriptor();



	opPool -> LockContainer();
	EMMediaPoolEntry* opEntry = opPool -> Find(vMediaEntryID);
	opPool -> UnlockContainer();
	//EMMediaPoolEntry* opEntry = opPool -> Find(vFileDescriptorID);
	
	m_vReturnValue = 0;
	if(opEntry != NULL)
		m_vReturnValue = opEntry -> GetLength(); //microseconds
	
	return static_cast<void*>(&m_vReturnValue);
}

bool MediaInfoCommandGetEntryDuration::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetEntryDuration::IsUndoable()
{
	return false;
}