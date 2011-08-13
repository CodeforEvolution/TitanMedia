#include "MediaInfoCommandGetClipMarkOut.h"

#include "EMMediaEngine.h"
#include "EMMediaPool.h"
#include "EMMediaPoolEntry.h"
#include "EMMediaProject.h"
#include "EMMediaClipRepository.h"
#include "EMMediaClip.h"
#include "EMMediaFormat.h"
#include "EMMediaUtility.h"

#include <list>

MediaInfoCommandGetClipMarkOut::MediaInfoCommandGetClipMarkOut()
{
}

void* MediaInfoCommandGetClipMarkOut::ExecuteE(void* p_upMediaClipID, void*, void*)
{
	//eo << "MediaInfoCommandGetClipMarkOut" << ef;
//	EMMediaEngine::Instance() -> LockProject();

	//TODO: Throw exception if fails:
	int32 vMediaClipID = *static_cast<int32*>(p_upMediaClipID);
	EMMediaProject* opProject = EMMediaEngine::Instance() -> GetMediaProject();
	EMMediaClipRepository* opAudioClipRepository = opProject -> GetAudioClipRepository();
	EMMediaClipRepository* opVideoClipRepository = opProject -> GetVideoClipRepository();
	EMMediaClipRepository* opVideoTransitionClipRepository = opProject -> GetVideoTransitionClipRepository();
	EMMediaClipRepository* opMIDIClipRepository = opProject -> GetMIDIClipRepository();
	
	EMMediaClip* opClip = NULL;
	opAudioClipRepository -> LockContainer();
	try
	{
		opClip = opAudioClipRepository -> Find(vMediaClipID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetClipMarkOut::ExecuteE");
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
			EMDebugger("ERROR! Exception in MediaInfoCommandGetClipMarkOut::ExecuteE");
		}
		opVideoClipRepository -> UnlockContainer();
	}
	opAudioClipRepository -> UnlockContainer();
	if(opClip == NULL)
	{
		opVideoTransitionClipRepository -> LockContainer();
		try
		{
			opClip = opVideoTransitionClipRepository -> Find(vMediaClipID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaInfoCommandGetClipMarkOut::ExecuteE");
		}
		opVideoTransitionClipRepository -> UnlockContainer();
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
			EMDebugger("ERROR! Exception in MediaInfoCommandGetClipMarkOut::ExecuteE");
		}
		opMIDIClipRepository -> UnlockContainer();
	}

	if(opClip == NULL)
		EMDebugger("ERROR! Could not find media clip id in clip repository!");
	
	m_vReturnValue = EMMediaUtility::Instance() -> FramesToTime(opClip -> GetMediaLength() - opClip -> GetMarkOutLength(), EMMediaUtility::Instance() -> GetSystemAudioFormat());

	//eo << m_vReturnValue << " = MediaInfoCommandGetClipMarkOut::ExecuteE()" << ef;

//	EMMediaEngine::Instance() -> UnlockProject();
	return static_cast<void*>(&m_vReturnValue);
}

bool MediaInfoCommandGetClipMarkOut::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetClipMarkOut::IsUndoable()
{
	return false;
}