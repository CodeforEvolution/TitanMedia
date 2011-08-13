#include "MediaInfoCommandGetEntryID.h"

#include "EMMediaEngine.h"
#include "EMMediaPool.h"
#include "EMMediaPoolEntry.h"
//#include "EMBeFileInputDescriptor.h"
#include "EMMediaProject.h"
#include "EMMediaClipRepository.h"
#include "EMMediaClip.h"

#include <list>

MediaInfoCommandGetEntryID::MediaInfoCommandGetEntryID()
{
}

/*
	AUDIO
	1 int32* media clip ID
	2 int16* array
	3 list<void*>*
		
		list:
			1 int32* capacity samples (2 bytes per sample!)
			2 int64* start time (us, media time)

*/
void* MediaInfoCommandGetEntryID::ExecuteE(void* p_upMediaClipID, void*, void*)
{
	//eo << "MediaInfoCommandGetEntryID" << ef;
	//TODO: Throw exception if fails:
	int32 vMediaClipID = *static_cast<int32*>(p_upMediaClipID);
	
	EMMediaPool* opPool = EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool();
	EMMediaProject* opProject = EMMediaEngine::Instance() -> GetMediaProject();
	EMMediaClipRepository* opAudioClipRepository = opProject -> GetAudioClipRepository();
	EMMediaClipRepository* opVideoClipRepository = opProject -> GetVideoClipRepository();
	
	EMMediaClip* opClip = NULL;
	opAudioClipRepository -> LockContainer();
	try
	{
		opClip = opAudioClipRepository -> Find(vMediaClipID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetEntryID::ExecuteE");
	}
	opAudioClipRepository -> UnlockContainer();
	if(opClip == NULL)
	{
		opVideoClipRepository -> LockContainer();
		try
		{
		opClip = opVideoClipRepository -> Find(vMediaClipID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaInfoCommandGetEntryID::ExecuteE");
		}
		opVideoClipRepository -> UnlockContainer();
	}

	if(opClip == NULL)
		EMDebugger("ERROR! Could not find media clip id in clip repository!");
	
	m_vReturnValue = opPool -> SearchFor(opClip -> GetFileName());
	return static_cast<void*>(&m_vReturnValue);
}

bool MediaInfoCommandGetEntryID::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetEntryID::IsUndoable()
{
	return false;
}