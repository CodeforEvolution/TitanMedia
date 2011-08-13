#include "MediaInfoCommandGetAudioEntryFormatString.h"

#include "EMMediaClip.h"
#include "EMMediaEngine.h"
#include "EMMediaPool.h"
#include "EMMediaPoolEntry.h"
#include "EMMediaPoolAudioEntry.h"
#include "EMMediaProject.h"
#include "EMMediaUtility.h"
#include "EMWinMediaUtility.h"

MediaInfoCommandGetAudioEntryFormatString::MediaInfoCommandGetAudioEntryFormatString()
{
}

void* MediaInfoCommandGetAudioEntryFormatString::ExecuteE(void* p_upMediaEntryID, void*, void*)
{
	//eo << "MediaInfoCommandGetAudioEntryFormatString" << ef;
	int32 vPoolEntryID = *static_cast<int32*>(p_upMediaEntryID);
	EMMediaPool* opPool = EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool();
	EMMediaPoolEntry* opEntry = NULL;
	opPool -> LockContainer();
	try
	{
		opEntry = opPool -> Find(vPoolEntryID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetAudioEntryFormatString::ExecuteE");
	}
	opPool -> UnlockContainer();
	if(opEntry == NULL)
		EMDebugger("ERROR! MediaInfoCommandGetAudioEntryFormatString could not find entry id!");
	if((opEntry -> GetType() & EM_TYPE_ANY_AUDIO) <= 0)
		EMDebugger("ERROR! MediaInfoCommandGetAudioEntryFormatString can only be called on AUDIO entries! This is obviously something else!");

	string oDescription = opEntry -> GetFormatString();
	for(int v = 0; v < oDescription.length() + 1; m_vpReturnValue[v++] = '\0')
		;
	memcpy(m_vpReturnValue, oDescription.c_str(), oDescription.length());
	return static_cast<void*>(m_vpReturnValue);
}

bool MediaInfoCommandGetAudioEntryFormatString::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetAudioEntryFormatString::IsUndoable()
{
	return false;
}
