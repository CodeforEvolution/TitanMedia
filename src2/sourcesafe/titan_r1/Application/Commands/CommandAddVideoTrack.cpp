#include "CommandAddVideoTrack.h"

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

const char* DEFAULT_VIDEO_TRACK_NAME = "Video Track";

CommandAddVideoTrack::CommandAddVideoTrack(TrackView* p_opTrackView, TrackInfo* p_opTrackInfo) :
m_opTrack(NULL),
m_opTrackInfo(p_opTrackInfo),
m_opTrackView(p_opTrackView)
{
}

CommandAddVideoTrack::~CommandAddVideoTrack()
{
	delete m_opTrack;
}

void CommandAddVideoTrack::ExecuteE()
{
	string oTrackName(DEFAULT_VIDEO_TRACK_NAME);
	// Assume that a new track is alwas added after all old tracks
	uint32 vNewTrackIndex = EMGTrackRepository::Instance() -> GetNrTracks();

	int32 vMediaTrackID = *(static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_CREATE_VIDEO_TRACK, const_cast<char*>(oTrackName.c_str())/*&oTrackName*/, &vNewTrackIndex, NULL)));
	;//cout_commented_out_4_release<<"SAVING TRACKID:" << vMediaTrackID << endl;
	if(m_opTrackInfo -> GetView() -> LockWindow())
	{
		EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_ADD_G_VIDEO_TRACK, &oTrackName, &vMediaTrackID);
		EMGTrackRepository::Instance() -> Notify(EM_TRACK_REPOSITORY_UPDATED); // Force an update
		m_opTrackInfo -> GetView() -> UnlockWindow();
	}
}

bool CommandAddVideoTrack::IsUndoable()
{
	return false;
}

bool CommandAddVideoTrack::RequiresParameters()
{
	return false;
}

void CommandAddVideoTrack::SetTrack(EMGTrack* p_opTrack)
{
	m_opTrack = p_opTrack;
}
