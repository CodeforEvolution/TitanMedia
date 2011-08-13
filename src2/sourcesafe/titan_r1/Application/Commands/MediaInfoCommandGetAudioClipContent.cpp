#include "MediaInfoCommandGetAudioClipContent.h"

#include "EMMediaEngine.h"
#include "EMMediaPool.h"
#include "EMMediaPoolEntry.h"
#include "EMMediaPoolAudioEntry.h"
//#include "EMBeFileInputDescriptor.h"
#include "EMMediaProject.h"
#include "EMMediaClip.h"

#include <list>

MediaInfoCommandGetAudioClipContent::MediaInfoCommandGetAudioClipContent()
{
}

void* MediaInfoCommandGetAudioClipContent::ExecuteE(void* p_upMediaPoolEntryID, void* p_upArray, void* p_upMoreParameters)
{
	//eo << "MediaInfoCommandGetAudioClipContent" << ef;
	//TODO: Throw exception if fails:
	int32 vMediaPoolEntryID = *static_cast<int32*>(p_upMediaPoolEntryID);
	uint16* vpArray = static_cast<uint16*>(p_upArray);
	list<void*>* opParameters = static_cast<list<void*>*>(p_upMoreParameters);
//	_ASSERT(opParameters -> size() == 2);
	int32 vArrayCapacityBytes = (*static_cast<int32*>(opParameters -> front())) * 2;
	int64 vMediaTime = *static_cast<int64*>(opParameters -> back());

	EMMediaPool* opPool = EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool();
	
	EMMediaPoolEntry* opEntry = NULL;
	opPool -> LockContainer();
	try
	{
		opEntry = opPool -> Find(vMediaPoolEntryID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetAudioClipContent::ExecuteE");
	}
	opPool -> UnlockContainer();
	if(opEntry != NULL)
	{
		if((opEntry -> GetType() & EM_TYPE_ANY_AUDIO) > 0)
		{
//			if(EMMediaEngine::Instance() -> GetMediaProject() -> LockAudio())
//			{
				static_cast<EMMediaPoolAudioEntry*>(opEntry) -> GetHiResContent(vpArray, vArrayCapacityBytes, vMediaTime);
//				EMMediaEngine::Instance() -> GetMediaProject() -> UnlockAudio();
//			}
		}
		else
			EMDebugger("ERROR! GetContent is only implemented for audio so far!");
	}
	else
	{
		;//emout_commented_out_4_release << "ERROR! Tried looking for " << vMediaPoolEntryID << endl;
		EMDebugger("ERROR! MediaInfoCommandGetAudioClipContent could not find pool entry ID");
	}

	if(vpArray == NULL)
		EMDebugger("ERROR! Array is NULL upon return from getaudioclipcontent");
	
	m_vReturnValue = vArrayCapacityBytes;
	return static_cast<void*>(&m_vReturnValue);
}

bool MediaInfoCommandGetAudioClipContent::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetAudioClipContent::IsUndoable()
{
	return false;
}