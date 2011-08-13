#include "CommandAddMIDITrack.h"

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

const char* DEFAULT_MIDI_TRACK_NAME = "MIDI Track";

CommandAddMIDITrack::CommandAddMIDITrack(TrackView* p_opTrackView, TrackInfo* p_opTrackInfo) :
m_opTrack(NULL),
m_opTrackInfo(p_opTrackInfo),
m_opTrackView(p_opTrackView)
{
}

CommandAddMIDITrack::~CommandAddMIDITrack()
{
	delete m_opTrack;
}

void CommandAddMIDITrack::ExecuteE()
{
	string oTrackName(DEFAULT_MIDI_TRACK_NAME);
	// Assume that a new track is alwas added after all old tracks
	uint32 vNewTrackIndex = EMGTrackRepository::Instance() -> GetNrTracks();

	int32 vMediaTrackID = *(static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_CREATE_MIDI_TRACK,  const_cast<char*>(oTrackName.c_str()), &vNewTrackIndex, NULL)));
//int32 vMediaTrackID(0);
	if(m_opTrackInfo -> GetView() -> LockWindow())
	{
		EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_ADD_G_MIDI_TRACK, &oTrackName, &vMediaTrackID);
		EMGTrackRepository::Instance() -> Notify(EM_TRACK_REPOSITORY_UPDATED); // Force an update
		m_opTrackInfo -> GetView() -> UnlockWindow();
	}
}

bool CommandAddMIDITrack::IsUndoable()
{
	return false;
}

bool CommandAddMIDITrack::RequiresParameters()
{
	return false;
}

void CommandAddMIDITrack::SetTrack(EMGTrack* p_opTrack)
{
	m_opTrack = p_opTrack;
}
