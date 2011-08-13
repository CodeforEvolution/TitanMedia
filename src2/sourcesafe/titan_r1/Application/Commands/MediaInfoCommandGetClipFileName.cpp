#include "MediaInfoCommandGetClipFileName.h"

#include "EMMediaEngine.h"
#include "EMMediaPool.h"
#include "EMMediaPoolEntry.h"
//#include "EMBeFileInputDescriptor.h"
#include "EMMediaProject.h"
#include "EMMediaClipRepository.h"
#include "EMMediaClip.h"
#include "EMMediaFormat.h"

#include <list>

MediaInfoCommandGetClipFileName::MediaInfoCommandGetClipFileName()
{
}

void* MediaInfoCommandGetClipFileName::ExecuteE(void* p_upMediaClipID, void*, void*)
{
	//eo << "MediaInfoCommandGetClipFileName" << ef;
	//TODO: Throw exception if fails:
	int32 vMediaClipID = *static_cast<int32*>(p_upMediaClipID);
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
		EMDebugger("ERROR! Exception in MediaInfoCommandGetClipFileName::ExecuteE");
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
			EMDebugger("ERROR! Exception in MediaInfoCommandGetClipFileName::ExecuteE");
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
			EMDebugger("ERROR! Exception in MediaInfoCommandGetClipFileName::ExecuteE");
		}
		opMIDIClipRepository -> UnlockContainer();
	}

	if(opClip == NULL)
		EMDebugger("ERROR! Could not find media clip id in clip repository!");
	
	m_vReturnValue = opClip -> GetFileName();
	return static_cast<void*>(&m_vReturnValue);
}

bool MediaInfoCommandGetClipFileName::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetClipFileName::IsUndoable()
{
	return false;
}