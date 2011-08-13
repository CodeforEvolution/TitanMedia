#include "MediaInfoCommandGetEntryFileName.h"

#include "EMMediaEngine.h"
#include "EMMediaPool.h"
#include "EMMediaPoolEntry.h"
//#include "EMBeFileInputDescriptor.h"
#include "EMMediaProject.h"
#include "EMMediaClip.h"

MediaInfoCommandGetEntryFileName::MediaInfoCommandGetEntryFileName()
{
}

void* MediaInfoCommandGetEntryFileName::ExecuteE(void* p_upMediaPoolEntryID, void*, void*)
{
	//eo << "MediaInfoCommandGetEntryFileName" << ef;

	string oString = "";

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
		EMDebugger("ERROR! Exception in MediaInfoCommandGetEntryFileName::ExecuteE");
	}
	opPool -> UnlockContainer();
	if(opEntry != NULL)
	{
		oString = opEntry -> GetFileName();
	}
	else
	{
//		char ch[80];
//		sprintf(ch, "ERROR! Could not find entry ID %d in media pool!", vMediaPoolEntryID);
//		opFileName = EM_new string(ch);
		oString = "";
	}
		//
	
	//EMMediaPoolEntry* opEntry = opPool -> Find(vFileDescriptorID);
	
//	int64* vDuration = EM_new int64;
//	*vDuration = 0;
	
//	if(opDesc != NULL)
//		*vDuration = opEntry -> GetLength(); //microseconds
	
	for(int v = 0; v < oString.length() +1; m_vpReturnValue[v++] = '\0')
		;
	memcpy(m_vpReturnValue, oString.c_str(), oString.length());
	return static_cast<void*>(&m_vpReturnValue);
}

bool MediaInfoCommandGetEntryFileName::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetEntryFileName::IsUndoable()
{
	return false;
}