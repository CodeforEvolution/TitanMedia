#include "MediaCommandSetClipMarkOutUndoable.h"

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

MediaCommandSetClipMarkOutUndoable::MediaCommandSetClipMarkOutUndoable()
	:	m_vClipID(-1),
		m_vOldMarkOut(0)
{
}

MediaCommandSetClipMarkOutUndoable::MediaCommandSetClipMarkOutUndoable(int32 p_vClipID, int64 p_vOldMarkOut)
	:	m_vClipID(p_vClipID),
		m_vOldMarkOut(p_vOldMarkOut)
{
}

void* MediaCommandSetClipMarkOutUndoable::ExecuteE(void* p_upClipID, void* p_upNewLength, void*)
{
	//eo << "MediaCommandSetClipMarkOutUndoable" << ef;
	int32 vClipID = *(static_cast<int32*>(p_upClipID));
	int64 vNewLength = *(static_cast<int64*>(p_upNewLength));

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
		EMDebugger("ERROR! Exception in MediaCommandSetClipMarkOutUndoable::ExecuteE");
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
			EMDebugger("ERROR! Exception in MediaCommandSetClipMarkOutUndoable::ExecuteE");
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
			EMDebugger("ERROR! Exception in MediaCommandSetClipMarkOutUndoable::ExecuteE");
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
			EMDebugger("ERROR! Exception in MediaCommandSetClipMarkOutUndoable::ExecuteE");
		}
		opMIDIClipRepository -> UnlockContainer();
	}

	if(opClip == NULL)
		return NULL; //TODO: Throw exception instead?

	m_vOldMarkOut = opClip -> GetMarkOutLength();
	m_vClipID = vClipID;

	opClip -> SetMarkOutLength(opClip -> GetMediaLength() - EMMediaUtility::Instance() -> TimeToFrames(vNewLength, EMMediaUtility::Instance() -> GetSystemAudioFormat()));
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);

	EMMediaTimer::Instance() -> SeekToFrame(EMMediaTimer::Instance() -> NowFrame());

	return NULL;
}

bool MediaCommandSetClipMarkOutUndoable::RequiresParameters()
{
	return true;
}

bool MediaCommandSetClipMarkOutUndoable::IsUndoable()
{
	return false;
}

EMCommand* MediaCommandSetClipMarkOutUndoable::CloneAndClear()
{
	return EM_new MediaCommandSetClipMarkOutUndoable(m_vClipID, m_vOldMarkOut);
}

void MediaCommandSetClipMarkOutUndoable::UndoE()
{
	EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(MEDIA_COMMAND_SET_CLIP_MARK_OUT, &m_vClipID, &m_vOldMarkOut, NULL);
}
