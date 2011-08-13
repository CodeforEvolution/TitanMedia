#include "MediaInfoCommandGetAudioEntryNumChannels.h"

#include "EMMediaEngine.h"
#include "EMMediaPool.h"
#include "EMMediaPoolEntry.h"
#include "EMMediaPoolAudioEntry.h"
#include "EMMediaProject.h"
#include "EMMediaClip.h"

MediaInfoCommandGetAudioEntryNumChannels::MediaInfoCommandGetAudioEntryNumChannels()
{
}

void* MediaInfoCommandGetAudioEntryNumChannels::ExecuteE(void* p_upPoolEntryID, void*, void*)
{
	//eo << "MediaInfoCommandGetAudioEntryNumChannels" << ef;
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
		EMDebugger("ERROR! Exception in MediaInfoCommandGetAudioEntryNumChannels::ExecuteE");
	}
	opPool -> UnlockContainer();
	if(opEntry == NULL)
		EMDebugger("ERROR! MediaInfoCommandGetAudioEntryNumChannels could not find entry id!");
	if((opEntry -> GetType() & EM_TYPE_ANY_AUDIO) <= 0)
		EMDebugger("ERROR! MediaInfoCommandGetAudioEntryNumChannels can only be called on AUDIO entries! This is obviously something else!");
	m_vReturnValue = static_cast<int32>(static_cast<EMMediaPoolAudioEntry*>(opEntry) -> GetNumChannels());
	return static_cast<void*>(&m_vReturnValue);
}

bool MediaInfoCommandGetAudioEntryNumChannels::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetAudioEntryNumChannels::IsUndoable()
{
	return false;
}