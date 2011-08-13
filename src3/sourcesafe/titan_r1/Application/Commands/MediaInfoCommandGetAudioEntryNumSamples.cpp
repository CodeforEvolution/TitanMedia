#include "MediaInfoCommandGetAudioEntryNumSamples.h"

#include "EMMediaClip.h"
#include "EMMediaEngine.h"
#include "EMMediaPool.h"
#include "EMMediaPoolEntry.h"
#include "EMMediaPoolAudioEntry.h"
#include "EMMediaProject.h"

MediaInfoCommandGetAudioEntryNumSamples::MediaInfoCommandGetAudioEntryNumSamples()
{
}

void* MediaInfoCommandGetAudioEntryNumSamples::ExecuteE(void* p_upPoolEntryID, void*, void*)
{
	//eo << "MediaInfoCommandGetAudioEntryNumSamples" << ef;
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
		EMDebugger("ERROR! Exception in MediaInfoCommandGetAudioEntryNumSamples::ExecuteE");
	}
	opPool -> UnlockContainer();
	if(opEntry == NULL)
		EMDebugger("ERROR! MediaInfoCommandGetAudioEntryNumSamples could not find entry id!");
	if((opEntry -> GetType() & EM_TYPE_ANY_AUDIO) <= 0)
		EMDebugger("ERROR! MediaInfoCommandGetAudioEntryNumSamples can only be called on AUDIO entries! This is obviously something else!");
	m_vReturnValue = static_cast<int32>(static_cast<EMMediaPoolAudioEntry*>(opEntry) -> GetNumSamples());
	
	return static_cast<void*>(&m_vReturnValue);
}

bool MediaInfoCommandGetAudioEntryNumSamples::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetAudioEntryNumSamples::IsUndoable()
{
	return false;
}