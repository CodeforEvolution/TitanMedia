#include "MediaCommandSetTrackKeyOffset.h"

#include "EMMediaEngine.h"
#include "EMCommandRepository.h"
#include "CommandIDs.h"
#include "EMMediaProject.h"
#include "EMMediaMIDITrack.h"
#include "EMMediaTimer.h"
#include "EMMediaTrackRepository.h"
#include "EMMediaUtility.h"

MediaCommandSetTrackKeyOffset::MediaCommandSetTrackKeyOffset(EMMediaMIDITrack* p_opRecentMIDITrack, int32 p_vRecentOffset)
	:	m_opRecentMIDITrack(p_opRecentMIDITrack),
		m_vRecentOffset(p_vRecentOffset)
{
}

void* MediaCommandSetTrackKeyOffset::ExecuteE(void* p_upMIDITrackID, void* p_upValue, void*)
{
	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();

	int32 vMIDITrackID = *static_cast<int32*>(p_upMIDITrackID);
	int32 vOffset = *(static_cast<int32*>(p_upValue));

	EMMediaMIDITrack* opTrack = NULL;
	opProject -> GetUsedTracks() -> LockContainer();
	opTrack = static_cast<EMMediaMIDITrack*>(opProject -> GetUsedTracks() -> Find(vMIDITrackID));
	opProject -> GetUsedTracks() -> UnlockContainer();

	if(opTrack == NULL)
	{
		opProject -> GetUnusedTracks() -> LockContainer();
		opTrack = static_cast<EMMediaMIDITrack*>(opProject -> GetUnusedTracks() -> Find(vMIDITrackID));
		opProject -> GetUnusedTracks() -> UnlockContainer();
	}

	if(opTrack == NULL)
		return NULL; //TODO: Throw exception instead?
	
	m_opRecentMIDITrack = opTrack;
	m_vRecentOffset = opTrack -> GetKeyOffset();
	
	vOffset = (vOffset >= -127 && vOffset <= 127 ? vOffset : (vOffset < -127 ? -127 : 127));
	opTrack -> SetKeyOffset(vOffset);
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
	EMMediaTimer::Instance() -> SeekToFrame(EMMediaTimer::Instance() -> NowFrame());
	return NULL;
}

bool MediaCommandSetTrackKeyOffset::RequiresParameters()
{
	return true;
}

void MediaCommandSetTrackKeyOffset::UndoE()
{
	if(m_opRecentMIDITrack != NULL)
	{
		int32 vMIDITrackID = m_opRecentMIDITrack -> GetID();
		int32 vOffsetValue = m_vRecentOffset;
		EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(MEDIA_COMMAND_SET_TRACK_KEY_OFFSET, &vMIDITrackID, &vOffsetValue);
	}
}

bool MediaCommandSetTrackKeyOffset::IsUndoable()
{
	return false;
}

EMCommand* MediaCommandSetTrackKeyOffset::CloneAndClear()
{
	return EM_new MediaCommandSetTrackKeyOffset(m_opRecentMIDITrack, m_vRecentOffset);
}

