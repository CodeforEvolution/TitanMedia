#include "CommandAddTransitionTrack.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMFactory.h"
#include "EMGTrack.h"
#include "EMGTrackRepository.h"
#include "EMMediaGlobals.h"
#include "EMMessageBox.h"
#include "EMMessages.h"
#include "EMView.h"
#include "TrackInfo.h"
#include "TrackView.h"
#include "TrackWindowStatusBar.h"
#include "TransitionGTrackInfo.h"
#include "VideoGTrackInfo.h"

#include <string>

const char* DEFAULT_TRANSITION_TRACK_NAME = "Transition Track";

CommandAddTransitionTrack::CommandAddTransitionTrack(TrackView* p_opTrackView, TrackInfo* p_opTrackInfo) :
m_opTrack(NULL),
m_opTrackInfo(p_opTrackInfo),
m_opTrackView(p_opTrackView)
{
}

CommandAddTransitionTrack::~CommandAddTransitionTrack()
{
	delete m_opTrack;
}

// Assumes that one single video track is selected
void CommandAddTransitionTrack::ExecuteE()
{
	vector<EMGTrack*> oSelectedTracks = EMGTrackRepository::Instance() -> GetSelectedTracks();
	EMGTrack* opParentTrack = oSelectedTracks.front();
	if(dynamic_cast<VideoGTrackInfo*>(opParentTrack -> GetTrackInfo()) -> GetTransitionTrack() != NULL)
	{
		EMFactory::Instance() -> CreateMessageBox() -> DisplayDialog(NULL, "This video track has already got a transítion track", "Error", EM_DIALOG_BUTTON_OK | EM_DIALOG_ICON_ERROR);
		return; // Can't add more than one transition track to a video track
	}

	int32 vParentTrackMediaID = opParentTrack -> GetTrackInfo() -> GetMediaTrackID();

	string oTrackName(DEFAULT_TRANSITION_TRACK_NAME);
	// Assume that a new track is alwas added after all old tracks
	uint32 vNewTrackIndex = EMGTrackRepository::Instance() -> GetNrTracks();

	int32 vMediaTrackID = *(static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_CREATE_VIDEO_TRANSITION_TRACK,  const_cast<char*>(oTrackName.c_str()), &vParentTrackMediaID)));
	EMGTrack* opTrack;
	if(m_opTrackInfo -> GetView() -> LockWindow())
	{
		opTrack = static_cast<EMGTrack*>(EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_ADD_G_TRANSITION_TRACK, &oTrackName, &vMediaTrackID));
		EMGTrackRepository::Instance() -> Notify(EM_TRACK_REPOSITORY_UPDATED); // Force an update
		opTrack -> GetTrackInfo() -> SetSelected(true);
		dynamic_cast<VideoGTrackInfo*>(opParentTrack -> GetTrackInfo()) -> SetTransitionTrack(opTrack);
		dynamic_cast<TransitionGTrackInfo*>(opTrack -> GetTrackInfo()) -> SetVideoTrack(opParentTrack);
		m_opTrackInfo -> GetView() -> UnlockWindow();
	}

	uint32 vBeforeIndex = EMGTrackRepository::Instance() -> GetTrackIndex(opParentTrack);
	uint32 vAfterIndex = vBeforeIndex;
	EMCommandRepository::Instance() -> ExecuteCommandNoUndo(COMMAND_MOVE_TRACKS, &vBeforeIndex, &vAfterIndex);
	delete EMCommandRepository::Instance() -> GetUndoClone(COMMAND_MOVE_TRACKS);
}

bool CommandAddTransitionTrack::IsUndoable()
{
	return false;
}

bool CommandAddTransitionTrack::RequiresParameters()
{
	return false;
}

void CommandAddTransitionTrack::SetTrack(EMGTrack* p_opTrack)
{
	m_opTrack = p_opTrack;
}

