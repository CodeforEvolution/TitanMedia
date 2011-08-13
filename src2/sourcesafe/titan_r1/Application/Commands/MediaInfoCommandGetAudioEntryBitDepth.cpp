#include "MediaInfoCommandGetAudioEntryBitDepth.h"

#include "EMMediaClip.h"
#include "EMMediaEngine.h"
#include "EMMediaPool.h"
#include "EMMediaPoolEntry.h"
#include "EMMediaPoolAudioEntry.h"
#include "EMMediaProject.h"

MediaInfoCommandGetAudioEntryBitDepth::MediaInfoCommandGetAudioEntryBitDepth()
{
}

void* MediaInfoCommandGetAudioEntryBitDepth::ExecuteE(void* p_upPoolEntryID, void*, void*)
{
	//eo << "MediaInfoCommandGetAudioEntryBitDepth" << ef;
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
		EMDebugger("ERROR! Exception in MediaInfoCommandGetAudioEntryBitDepth::ExecuteE");
	}
	opPool -> UnlockContainer();
	if(opEntry == NULL)
		EMDebugger("ERROR! MediaInfoCommandGetAudioEntryNumSamples could not find entry id!");
	if((opEntry -> GetType() & EM_TYPE_ANY_AUDIO) <= 0)
		EMDebugger("ERROR! MediaInfoCommandGetAudioEntryBitDepth can only be called on AUDIO entries! This is obviously something else!");
	m_vReturnValue = static_cast<int32>(static_cast<EMMediaPoolAudioEntry*>(opEntry) -> GetFormat() -> m_vBytesPerSample * 8);
	
	return static_cast<void*>(&m_vReturnValue);
}

bool MediaInfoCommandGetAudioEntryBitDepth::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetAudioEntryBitDepth::IsUndoable()
{
	return false;
}