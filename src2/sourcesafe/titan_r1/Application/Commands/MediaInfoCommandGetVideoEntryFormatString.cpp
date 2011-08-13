#include "MediaInfoCommandGetVideoEntryFormatString.h"

#include "EMMediaClip.h"
#include "EMMediaEngine.h"
#include "EMMediaPool.h"
#include "EMMediaPoolEntry.h"
#include "EMMediaPoolAudioEntry.h"
#include "EMMediaProject.h"

MediaInfoCommandGetVideoEntryFormatString::MediaInfoCommandGetVideoEntryFormatString()
{
}

void* MediaInfoCommandGetVideoEntryFormatString::ExecuteE(void* p_upMediaEntryID, void*, void*)
{
	//eo << "MediaInfoCommandGetVideoEntryFormatString" << ef;
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
		EMDebugger("ERROR! Exception in MediaInfoCommandGetVideoEntryFormatString::ExecuteE");
	}
	opPool -> UnlockContainer();
	if(opEntry == NULL)
		EMDebugger("ERROR! MediaInfoCommandGetVideoEntryFormatString could not find entry id!");
	if((opEntry -> GetType() & EM_TYPE_ANY_AUDIO) <= 0)
		EMDebugger("ERROR! MediaInfoCommandGetVideoEntryFormatString can only be called on VIDEO entries! This is obviously something else!");

	string oDescription = opEntry -> GetFormatString();
	for(int v = 0; v < oDescription.length() + 1; m_vpReturnValue[v++] = '\0')
		;
	memcpy(m_vpReturnValue, oDescription.c_str(), oDescription.length());
	return static_cast<void*>(m_vpReturnValue);
}

bool MediaInfoCommandGetVideoEntryFormatString::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetVideoEntryFormatString::IsUndoable()
{
	return false;
}
