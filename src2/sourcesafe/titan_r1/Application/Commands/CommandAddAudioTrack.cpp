#include "CommandAddAudioTrack.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGTrack.h"
#include "EMGTrackRepository.h"
#include "EMMediaGlobals.h"
#include "EMMessages.h"
#include "EMView.h"
#include "TrackInfo.h"
#include "TrackView.h"
#include "TrackWindowStatusBar.h"

#include <string>

const char* DEFAULT_AUDIO_TRACK_NAME = "Audio Track";

CommandAddAudioTrack::CommandAddAudioTrack(TrackView* p_opTrackView, TrackInfo* p_opTrackInfo) :
m_opTrack(NULL),
m_opTrackInfo(p_opTrackInfo),
m_opTrackView(p_opTrackView)
{
}

CommandAddAudioTrack::~CommandAddAudioTrack()
{
	delete m_opTrack;
}

void CommandAddAudioTrack::ExecuteE()
{
	string oTrackName(DEFAULT_AUDIO_TRACK_NAME);
	// Assume that a new track is alwas added after all old tracks
	uint32 vNewTrackIndex = EMGTrackRepository::Instance() -> GetNrTracks();

	int32 vMediaTrackID = *(static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_CREATE_AUDIO_TRACK, const_cast<char*>(oTrackName.c_str())/*&oTrackName*/, &vNewTrackIndex, NULL)));
	if(m_opTrackInfo -> GetView() -> LockWindow())
	{
		EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_ADD_G_AUDIO_TRACK, &oTrackName, &vMediaTrackID);
		EMGTrackRepository::Instance() -> Notify(EM_TRACK_REPOSITORY_UPDATED); // Force an update
		m_opTrackInfo -> GetView() -> UnlockWindow();
	}
}

bool CommandAddAudioTrack::IsUndoable()
{
	return false;
}

bool CommandAddAudioTrack::RequiresParameters()
{
	return false;
}

void CommandAddAudioTrack::SetTrack(EMGTrack* p_opTrack)
{
	m_opTrack = p_opTrack;
}

