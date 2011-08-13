#include "MediaInfoCommandGetClipNumChannels.h"

#include "EMMediaEngine.h"
#include "EMMediaPool.h"
#include "EMMediaPoolEntry.h"
#include "EMMediaPoolAudioEntry.h"
//#include "EMBeFileInputDescriptor.h"
#include "EMMediaProject.h"
#include "EMMediaClipRepository.h"
#include "EMMediaClip.h"

#include <list>

MediaInfoCommandGetClipNumChannels::MediaInfoCommandGetClipNumChannels()
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
void* MediaInfoCommandGetClipNumChannels::ExecuteE(void* p_upMediaClipID, void*, void*)
{
	//eo << "MediaInfoCommandGetClipNumChannels" << ef;
	//TODO: Throw exception if fails:
	int32 vMediaClipID = *static_cast<int32*>(p_upMediaClipID);
	
	EMMediaPool* opPool = EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool();
	EMMediaProject* opProject = EMMediaEngine::Instance() -> GetMediaProject();
	EMMediaClipRepository* opAudioClipRepository = opProject -> GetAudioClipRepository();
	EMMediaClipRepository* opVideoClipRepository = opProject -> GetVideoClipRepository();
	EMMediaClipRepository* opMIDIClipRepository = opProject -> GetMIDIClipRepository();
	
	EMMediaClip* opClip = NULL;
	opAudioClipRepository -> LockContainer();
	try
	{
		opClip = opAudioClipRepository -> Find(vMediaClipID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetClipNumChannels::ExecuteE");
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
			EMDebugger("ERROR! Exception in MediaInfoCommandGetClipNumChannels::ExecuteE");
		}
		opVideoClipRepository -> UnlockContainer();
	}
	if(opClip == NULL)
	{
		opMIDIClipRepository -> LockContainer();
		try
		{
			opClip = opMIDIClipRepository -> Find(vMediaClipID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaInfoCommandGetClipNumChannels::ExecuteE");
		}
		opMIDIClipRepository -> UnlockContainer();
	}

	if(opClip == NULL)
		EMDebugger("ERROR! Could not find media clip id in clip repository!");
	
	int32 vPoolEntryID = opPool -> SearchFor(opClip -> GetFileName());
	if(vPoolEntryID == -1)
		EMDebugger("ERROR! Could not locate the file in the media pool!");
		
	EMMediaPoolEntry* opEntry = NULL;
	opPool -> LockContainer();
	try
	{
		opEntry = opPool -> Find(vPoolEntryID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetClipNumChannels::ExecuteE");
	}
	opPool -> UnlockContainer();
	if(opEntry != NULL)
	{
		if((opClip -> GetType() & EM_TYPE_ANY_AUDIO) > 0)
		{
			m_vReturnValue = static_cast<int32>(static_cast<EMMediaPoolAudioEntry*>(opEntry) -> GetNumChannels());
			return static_cast<void*>(&m_vReturnValue);
		}
		else
		{
			EMDebugger("ERROR! MediaInfoCommandGetClipNumChannels is only implemented for audio!");
		}
	}

	return static_cast<void*>(NULL);
}

bool MediaInfoCommandGetClipNumChannels::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetClipNumChannels::IsUndoable()
{
	return false;
}