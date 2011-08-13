#include "MediaCommandSetClipMarkOut.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaTrack.h"
#include "EMMediaClipRepository.h"
#include "EMMediaClip.h"
#include "EMMediaTimer.h"
#include "EMMediaFormat.h"
#include "EMMediaUtility.h"

MediaCommandSetClipMarkOut::MediaCommandSetClipMarkOut()
{
}

void* MediaCommandSetClipMarkOut::ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void*)
{
	//eo << "MediaCommandSetClipMarkOut" << ef;
	int32 vClipID = *(static_cast<int32*>(p_opParameterOne));
	int64 vNewLength = *(static_cast<int64*>(p_opParameterTwo));

	eo << "MediaCommandSetClipMarkOut::ExecuteE(" << vClipID << ", " << vNewLength << ");" << ef;

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
		EMDebugger("ERROR! Exception in MediaCommandSetClipMarkOut::ExecuteE");
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
			EMDebugger("ERROR! Exception in MediaCommandSetClipMarkOut::ExecuteE");
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
			EMDebugger("ERROR! Exception in MediaCommandSetClipMarkOut::ExecuteE");
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
			EMDebugger("ERROR! Exception in MediaCommandSetClipMarkOut::ExecuteE");
		}
		opVideoTransitionClipRepository -> UnlockContainer();
	}

	if(opClip == NULL)
		return NULL; //TODO: Throw exception instead?

	opClip -> SetMarkOutLength(opClip -> GetMediaLength() - EMMediaUtility::Instance() -> TimeToFrames(vNewLength, EMMediaUtility::Instance() -> GetSystemAudioFormat()));
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);

	EMMediaTimerState eMode = opTimer -> GetState();
	if(eMode == EM_PLAYING || eMode == EM_RECORDING)
		opTimer -> SetState(EM_STOPPED);
	opTimer -> SeekToFrame(opTimer -> NowFrame());
	if(eMode == EM_PLAYING || eMode == EM_RECORDING)
		opTimer -> SetState((eMode == EM_RECORDING ? EM_COUNTING_IN_FOR_RECORDING : EM_COUNTING_IN_FOR_PLAYBACK));

	return NULL;
}

bool MediaCommandSetClipMarkOut::RequiresParameters()
{
	return true;
}

bool MediaCommandSetClipMarkOut::IsUndoable()
{
	return false;
}
