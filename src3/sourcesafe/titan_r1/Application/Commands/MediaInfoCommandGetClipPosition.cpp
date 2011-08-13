#include "MediaInfoCommandGetClipPosition.h"

#include "EMMediaEngine.h"
#include "EMMediaPool.h"
#include "EMMediaPoolEntry.h"
#include "EMMediaProject.h"
#include "EMMediaClipRepository.h"
#include "EMMediaClip.h"
#include "EMMediaFormat.h"
#include "EMMediaUtility.h"

#include <list>

MediaInfoCommandGetClipPosition::MediaInfoCommandGetClipPosition()
{
}

void* MediaInfoCommandGetClipPosition::ExecuteE(void* p_upMediaClipID, void*, void*)
{
	//eo << "MediaInfoCommandGetClipPosition" << ef;
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
		EMDebugger("ERROR! Exception in MediaInfoCommandGetClipPosition::ExecuteE");
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
			EMDebugger("ERROR! Exception in MediaInfoCommandGetClipPosition::ExecuteE");
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
			EMDebugger("ERROR! Exception in MediaInfoCommandGetClipPosition::ExecuteE");
		}
		opMIDIClipRepository -> UnlockContainer();
	}
	if(opClip == NULL)
	{
		opVideoTransitionClipRepository -> LockContainer();
		try
		{
			opClip = opVideoTransitionClipRepository -> Find(vMediaClipID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaInfoCommandGetClipPosition::ExecuteE");
		}
		opVideoTransitionClipRepository -> UnlockContainer();
	}

	if(opClip == NULL)
		EMDebugger("ERROR! Could not find media clip id in clip repository!");
	
	EMMediaFormat oFormat(EM_TYPE_ANY_AUDIO);
//	int64 vMediaTime = EMMediaUtility::Instance() -> FramesToTime(opClip -> GetMarkInFrame(), &oFormat);
	int64 vMediaTime = EMMediaUtility::Instance() -> FramesToTime(opClip -> GetStart(), &oFormat);
	m_vReturnValue = vMediaTime;

	eo << "MediaInfoCommandGetClipPosition::Exec " << m_vReturnValue << ef;
	return static_cast<void*>(&m_vReturnValue); 
}

bool MediaInfoCommandGetClipPosition::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetClipPosition::IsUndoable()
{
	return false;
}