#include "MediaCommandResizeClip.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaTrack.h"
#include "EMMediaClipRepository.h"
#include "EMMediaClip.h"
#include "EMMediaTimer.h"
#include "EMMediaFormat.h"
#include "EMMediaTrackRepository.h"
#include "EMMediaUtility.h"
#include "EMMediaVideoTransitionClip.h"

MediaCommandResizeClip::MediaCommandResizeClip(EMVideoTransitionClip* p_opClip, int64 p_vOldStart, int64 p_vOldStop)
	:	m_opClip(p_opClip),
		m_vOldStart(p_vOldStart),
		m_vOldStop(p_vOldStop)
{
}

void* MediaCommandResizeClip::ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree)
{
	//eo << "MediaCommandResizeClip" << ef;
	bool vTrackWasEmpty = false;
	int32 vClipID   = *(static_cast<int32*>(p_opParameterOne));
	int64 vNewStart = *(static_cast<int64*>(p_opParameterTwo));
	int64 vNewStop  = *(static_cast<int64*>(p_opParameterThree));

	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();
	EMMediaTimer* opTimer = EMMediaTimer::Instance();
 
	EMMediaClipRepository* opVideoTransitionClipRepository = opProject -> GetVideoTransitionClipRepository();

	opVideoTransitionClipRepository -> LockContainer();
	try
	{
		m_opClip = static_cast<EMVideoTransitionClip*>(opVideoTransitionClipRepository -> Find(vClipID));

		opVideoTransitionClipRepository -> UnlockContainer();


		if(m_opClip != NULL)
		{
			m_vOldStart = m_opClip -> GetStart();
			m_vOldStop  = m_opClip -> GetStop();
		}
		else
			return NULL; 

		EMMediaTimerState eMode = opTimer -> GetState();
		if(eMode == EM_PLAYING || eMode == EM_RECORDING)
			opTimer -> SetState(EM_STOPPED);
		opTimer -> SeekToFrame(opTimer -> NowFrame());

		int64 vNewFrameStart = EMMediaUtility::Instance() -> TimeToFrames(vNewStart, EMMediaUtility::Instance() -> GetSystemAudioFormat());
		int64 vNewFrameStop  = EMMediaUtility::Instance() -> TimeToFrames(vNewStop, EMMediaUtility::Instance() -> GetSystemAudioFormat());

		m_opClip -> SetStart(vNewFrameStart);
		m_opClip -> SetStop(vNewFrameStop);

		EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);

		if(eMode == EM_PLAYING || eMode == EM_RECORDING)
			opTimer -> SetState((eMode == EM_RECORDING ? EM_COUNTING_IN_FOR_RECORDING : EM_COUNTING_IN_FOR_PLAYBACK));
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaCommandResizeClip::ExecuteE");
	}

	return NULL;
}

bool MediaCommandResizeClip::RequiresParameters()
{
	return true;
}

bool MediaCommandResizeClip::IsUndoable()
{
	return false;
}

EMCommand* MediaCommandResizeClip::CloneAndClear()
{
	return EM_new MediaCommandResizeClip(m_opClip, m_vOldStart, m_vOldStop);
}

void MediaCommandResizeClip::UndoE()
{
	if(m_opClip != NULL)
	{
		m_opClip -> SetStart(m_vOldStart);
		m_opClip -> SetStop(m_vOldStop);
	}
}
