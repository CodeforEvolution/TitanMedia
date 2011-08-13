#include "MediaInfoCommandGetClipMarkIn.h"

#include "EMMediaEngine.h"
#include "EMMediaPool.h"
#include "EMMediaPoolEntry.h"
#include "EMMediaProject.h"
#include "EMMediaClipRepository.h"
#include "EMMediaClip.h"
#include "EMMediaFormat.h"
#include "EMMediaUtility.h"

#include <list>

MediaInfoCommandGetClipMarkIn::MediaInfoCommandGetClipMarkIn()
{
}

MediaInfoCommandGetClipMarkIn::~MediaInfoCommandGetClipMarkIn()
{
}

void* MediaInfoCommandGetClipMarkIn::ExecuteE(void* p_upMediaClipID, void*, void*)
{
	//eo << "MediaInfoCommandGetClipMarkIn" << ef;
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
		EMDebugger("ERROR! Exception in MediaInfoCommandGetClipMarkIn::ExecuteE");
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
			EMDebugger("ERROR! Exception in MediaInfoCommandGetClipMarkIn::ExecuteE");
		}
		opVideoClipRepository -> UnlockContainer();
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
			EMDebugger("ERROR! Exception in MediaInfoCommandGetClipMarkIn::ExecuteE");
		}
		opVideoTransitionClipRepository -> UnlockContainer();
	}	if(opClip == NULL)
	{
		opMIDIClipRepository -> LockContainer();
		try
		{
			opClip = opMIDIClipRepository -> Find(vMediaClipID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaInfoCommandGetClipMarkIn::ExecuteE");
		}
		opMIDIClipRepository -> UnlockContainer();
	}

	if(opClip == NULL)
	{
		;//cout_commented_out_4_release << "Searched for " << vMediaClipID << endl;
		EMDebugger("ERROR! Could not find media clip id in clip repository!");
	}
	int32 vDummy = opClip -> GetMarkInLength();

	//eo << "MediaCommandGetClipMarkIn value: " << vDummy << ef;

	m_vReturnValue = EMMediaUtility::Instance() -> FramesToTime(opClip -> GetMarkInLength(), EMMediaUtility::Instance() -> GetSystemAudioFormat());

//	EMMediaEngine::Instance() -> UnlockProject();

	return static_cast<void*>(&m_vReturnValue);
}

bool MediaInfoCommandGetClipMarkIn::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetClipMarkIn::IsUndoable()
{
	return false;
}