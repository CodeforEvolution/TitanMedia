#include "MediaCommandSetClipMarkInUndoable.h"

#include "EMMediaEngine.h"
#include "EMCommandRepository.h"
#include "CommandIDs.h"
#include "EMMediaProject.h"
#include "EMMediaTrack.h"
#include "EMMediaClipRepository.h"
#include "EMMediaClip.h"
#include "EMMediaTimer.h"
#include "EMMediaFormat.h"
#include "EMMediaUtility.h"

MediaCommandSetClipMarkInUndoable::MediaCommandSetClipMarkInUndoable()
	:	m_vClipID(-1),
		m_vOldMarkIn(0)
{
}

MediaCommandSetClipMarkInUndoable::MediaCommandSetClipMarkInUndoable(int32 p_vClipID, int64 p_vMarkIn)
	:	m_vClipID(p_vClipID),
		m_vOldMarkIn(p_vMarkIn)
{
}

void* MediaCommandSetClipMarkInUndoable::ExecuteE(void* p_upClipID, void* p_upNewOffset, void* p_upIsUndoableMove)
{
	int32 vClipID = *(static_cast<int32*>(p_upClipID));
	int64 vNewOffset = *(static_cast<int64*>(p_upNewOffset));
	bool vIsAnUndoableMove = (p_upIsUndoableMove == NULL ? false : *(static_cast<bool*>(p_upIsUndoableMove)));

	EMMediaClip* opClip = NULL;

	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();
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
		EMDebugger("ERROR! Exception in MediaCommandSetClipMarkInUndoable::ExecuteE");
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
			EMDebugger("ERROR! Exception in MediaCommandSetClipMarkInUndoable::ExecuteE");
		}
		opVideoClipRepository -> UnlockContainer();
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
			EMDebugger("ERROR! Exception in MediaCommandSetClipMarkInUndoable::ExecuteE");
		}
		opVideoTransitionClipRepository -> UnlockContainer();
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
			EMDebugger("ERROR! Exception in MediaCommandSetClipMarkInUndoable::ExecuteE");
		}
		opMIDIClipRepository -> UnlockContainer();
	}

	if(opClip == NULL)
		return NULL; //TODO: Throw exception instead?

	m_vOldMarkIn = opClip -> GetMarkInLength();
	m_vClipID = vClipID;

	opClip -> SetMarkInLength(EMMediaUtility::Instance() -> TimeToFrames(vNewOffset, EMMediaUtility::Instance() -> GetSystemAudioFormat()));
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);

	int32 vDummy = (EMMediaUtility::Instance() -> TimeToFrames(vNewOffset, EMMediaUtility::Instance() -> GetSystemAudioFormat()));

	eo << "MediaCommandSetClipMarkInUndoable: New value: "<< vDummy << ef;


	EMMediaTimer::Instance() -> SeekToFrame(EMMediaTimer::Instance() -> NowFrame());
	return NULL;
}

bool MediaCommandSetClipMarkInUndoable::RequiresParameters()
{
	return true;
}

bool MediaCommandSetClipMarkInUndoable::IsUndoable()
{
	return false;
}

EMCommand* MediaCommandSetClipMarkInUndoable::CloneAndClear()
{
	return EM_new MediaCommandSetClipMarkInUndoable(m_vClipID, m_vOldMarkIn);
}

void MediaCommandSetClipMarkInUndoable::UndoE()
{
	if(m_vClipID != -1)
		EMMediaEngine::Instance() -> GetCommandRepository()-> ExecuteCommand(MEDIA_COMMAND_SET_CLIP_MARK_IN, &m_vClipID, &m_vOldMarkIn, NULL);
}
