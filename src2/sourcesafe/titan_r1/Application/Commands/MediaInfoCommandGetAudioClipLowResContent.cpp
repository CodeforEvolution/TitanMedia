#include "MediaInfoCommandGetAudioClipLowResContent.h"

#include "EMMediaEngine.h"
#include "EMMediaPool.h"
#include "EMMediaPoolEntry.h"
#include "EMMediaPoolAudioEntry.h"
//#include "EMBeFileInputDescriptor.h"
#include "EMMediaProject.h"
#include "EMMediaClip.h"

#include <list>

MediaInfoCommandGetAudioClipLowResContent::MediaInfoCommandGetAudioClipLowResContent()
{
}

/*
	1.	media clip id int32*
	2.	array, int8*
	3.	array length int64* (num slots)
	
*/
void* MediaInfoCommandGetAudioClipLowResContent::ExecuteE(void* p_upMediaPoolEntryID, void* p_upArray, void* p_upArrayNumSlots)
{
	//eo << "MediaInfoCommandGetAudioClipLowResContent" << ef;
	int32 vPoolEntryID = *static_cast<int32*>(p_upMediaPoolEntryID);
	uint8* vpArray = static_cast<uint8*>(p_upArray);
	int64 vArrayLength = *static_cast<int64*>(p_upArrayNumSlots);

	EMMediaPool* opPool = EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool();
	
	opPool -> LockContainer();
	EMMediaPoolEntry* opEntry = opPool -> Find(vPoolEntryID);
	opPool -> UnlockContainer();
	if(opEntry != NULL)
	{
		if((opEntry -> GetType() & EM_TYPE_ANY_AUDIO) > 0)
			static_cast<EMMediaPoolAudioEntry*>(opEntry) -> GetLowResContent(vpArray, vArrayLength);
		else
			EMDebugger("ERROR! MediaInfoCommandGetAudioClipLowResContent is only implemented for audio so far!");
	}
	else
		EMDebugger("ERROR! MediaInfoCommandGetAudioClipLowResContent could not find pool entry ID");

	m_vReturnValue = vArrayLength;
	return static_cast<void*>(&m_vReturnValue);
}

bool MediaInfoCommandGetAudioClipLowResContent::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetAudioClipLowResContent::IsUndoable()
{
	return false;
}