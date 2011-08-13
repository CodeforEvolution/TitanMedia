#include "MediaCommandDropMedia.h"

#include "EMMediaEngine.h"
#include "EMCommandRepository.h"
#include "CommandIDs.h"
#include "EMMediaProject.h"
#include "EMFileInputDescriptor.h"
#include "EMMediaTrack.h"
#include "EMMediaTimer.h"
#include "EMMediaTrackRepository.h"
#include "EMMediaPool.h"
#include "EMSettingsRepository.h"
#include "EMSettingIDs.h"
#include "EMRealtimeOutputDescriptor.h"

MediaCommandDropMedia::MediaCommandDropMedia()
	:	m_vNewClipID(-1)
{
}

MediaCommandDropMedia::MediaCommandDropMedia(int32 p_vNewClipID)
	:	m_vNewClipID(p_vNewClipID)
{
}

void* MediaCommandDropMedia::ExecuteE(void* p_upEntryID, void* p_upTrackID, void* p_upAtTime)
{
	//eo << "MediaCommandDropMedia" << ef;
	bool vTrackWasEmpty = false;
	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();
	int32 vEntryID = *(static_cast<int32*>(p_upEntryID));
	int32 vTrackID = *(static_cast<int32*>(p_upTrackID));
	int64 vAtTime = *(static_cast<int32*>(p_upAtTime));
	vAtTime = static_cast<int64>(((float) *(static_cast<int32*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_FRAMERATE))) * vAtTime) / 1000000.0);
	EMFileInputDescriptor* opDescriptor = opEngine -> GetMediaProject() -> GetMediaPool() -> InstantiateEntryE(vEntryID);
	if(opDescriptor == NULL)
	{
		eo << "Uhh?" << ef;
		EMDebugger("ERROR! Could not instantiate pool entry!");
	}

	EMMediaTrack* opTrack = NULL;
	opProject -> GetUsedTracks() -> LockContainer();
	try
	{
		opTrack = opProject -> GetUsedTracks() -> Find(vTrackID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaCommandDropMedia::ExecuteE");
	}
	opProject -> GetUsedTracks() -> UnlockContainer();
	if(opTrack == NULL)
	{
		opProject -> GetUnusedTracks() -> LockContainer();
		try
		{
			opTrack = opProject -> GetUnusedTracks() -> Find(vTrackID);
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in MediaCommandDropMedia::ExecuteE");
		}
		opProject -> GetUnusedTracks() -> UnlockContainer();
		vTrackWasEmpty = true;
	}
	string oClipName = opDescriptor -> GetName();
	EMMediaTimer* opTimer = EMMediaTimer::Instance();

	EMMediaTimerState eMode = opTimer -> GetState();
	if(eMode == EM_PLAYING || eMode == EM_RECORDING)
		opTimer -> SetState(EM_STOPPED);
	opTimer -> SeekToFrame(opTimer -> NowFrame());
	
	m_vReturnValue = opTrack -> CreateClip(vAtTime, oClipName, opDescriptor);
	
	if(vTrackWasEmpty)
	{
		opProject -> GetUnusedTracks() -> RemoveTrack(opTrack);
		opProject -> GetUsedTracks() -> AddTrack(opTrack);
	}

	EMRealtimeOutputDescriptor* opOut = EMOutputRepository::Instance() -> FindClockMaster();
	if(opOut != NULL)
	{
		opOut -> RegisterAsClockMaster();
		EMMediaTimer::Instance() -> RegisterClockMaster(opOut);
	}

	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
	m_vNewClipID = m_vReturnValue;
	if(eMode == EM_PLAYING || eMode == EM_RECORDING)
		opTimer -> SetState((eMode == EM_RECORDING ? EM_COUNTING_IN_FOR_RECORDING : EM_COUNTING_IN_FOR_PLAYBACK));
	return static_cast<void*>(&m_vReturnValue);
}

bool MediaCommandDropMedia::RequiresParameters()
{
	return true;
}

bool MediaCommandDropMedia::IsUndoable()
{
	return false;
}

EMCommand* MediaCommandDropMedia::CloneAndClear()
{
	return EM_new MediaCommandDropMedia(m_vNewClipID);
}

void MediaCommandDropMedia::UndoE()
{
	if(m_vNewClipID != -1)
		EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(MEDIA_COMMAND_DELETE_CLIP, &m_vNewClipID, NULL, NULL);
}