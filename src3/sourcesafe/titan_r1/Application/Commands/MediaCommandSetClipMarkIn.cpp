#include "MediaCommandSetClipMarkIn.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaTrack.h"
#include "EMMediaClipRepository.h"
#include "EMMediaClip.h"
#include "EMMediaTimer.h"
#include "EMMediaFormat.h"
#include "EMMediaUtility.h"

MediaCommandSetClipMarkIn::MediaCommandSetClipMarkIn()
{
}

void* MediaCommandSetClipMarkIn::ExecuteE(void* p_upClipID, void* p_upNewOffset, void*)
{
	//eo << "MediaCommandSetClipMarkIn" << ef;
	int32 vClipID = *(static_cast<int32*>(p_upClipID));
	int64 vNewOffset = *(static_cast<int64*>(p_upNewOffset));
 
	EMMediaClip* opClip = NULL;

	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();
	EMMediaTimer* opTimer = EMMediaTimer::Instance();
	EMMediaClipRepository* opAudioClipRepository = opProject -> GetAudioClipRepository();
	EMMediaClipRepository* opVideoClipRepository = opProject -> GetVideoClipRepository();
	EMMediaClipRepository* opVideoTransitionClipRepository = opProject -> GetVideoTransitionClipRepository();
	EMMediaClipRepository* opMIDIClipRepository = opProject -> GetMIDIClipRepository();
	
	opAudioClipRepository -> LockContainer();
	try
	{
		opClip = opAudioClipRepository -> Find(vClipID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaCommandSetClipMarkIn::ExecuteE");
	}
	opAudioClipRepository -> UnlockContainer();
	if(opClip == NULL)
	{
		opVideoClipRepository -> LockContainer();
		try
		{
			opClip = opVideoClipRepository -> Find(vClipID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandSetClipMarkIn::ExecuteE");
		}
		opVideoClipRepository -> UnlockContainer();
	}
	if(opClip == NULL)
	{
		opMIDIClipRepository -> LockContainer();
		try
		{
			opClip = opMIDIClipRepository -> Find(vClipID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandSetClipMarkIn::ExecuteE");
		}
		opMIDIClipRepository -> UnlockContainer();
	}
	if(opClip == NULL)
	{
		opVideoTransitionClipRepository -> LockContainer();
		try
		{
			opClip = opVideoTransitionClipRepository -> Find(vClipID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandSetClipMarkIn::ExecuteE");
		}
		opVideoTransitionClipRepository -> UnlockContainer();
	}

	if(opClip == NULL)
		return NULL; //TODO: Throw exception instead?

	opClip -> SetMarkInLength(EMMediaUtility::Instance() -> TimeToFrames(vNewOffset, EMMediaUtility::Instance() -> GetSystemAudioFormat()));
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);

	int32 vDummy = (EMMediaUtility::Instance() -> TimeToFrames(vNewOffset, EMMediaUtility::Instance() -> GetSystemAudioFormat()));

	eo << "MediaCommandSetClipMarkIn: New value: " << vDummy << ef;

	EMMediaTimerState eMode = opTimer -> GetState();
	if(eMode == EM_PLAYING || eMode == EM_RECORDING)
		opTimer -> SetState(EM_STOPPED);
	opTimer -> SeekToFrame(opTimer -> NowFrame());
	if(eMode == EM_PLAYING || eMode == EM_RECORDING)
		opTimer -> SetState((eMode == EM_RECORDING ? EM_COUNTING_IN_FOR_RECORDING : EM_COUNTING_IN_FOR_PLAYBACK));
	return NULL;
}

bool MediaCommandSetClipMarkIn::RequiresParameters()
{
	return true;
}

bool MediaCommandSetClipMarkIn::IsUndoable()
{
	return false;
}
