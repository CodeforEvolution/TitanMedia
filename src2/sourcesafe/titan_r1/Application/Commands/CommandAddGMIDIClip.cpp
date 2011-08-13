#include "CommandAddGMIDIClip.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGClipRepository.h"
#include "EMGTrack.h"
#include "EMGTrackRepository.h"
#include "EMView.h"
#include "TrackView.h"
#include "TrackWindowStatusBar.h"

#include <string>

const char* DEFAULT_MIDI_CLIP_NAME = "MIDI Clip";

CommandAddGMIDIClip::CommandAddGMIDIClip(TrackView* p_opTrackView, TrackInfo* p_opTrackInfo, EMGClip* p_opClip) :
m_opClip(p_opClip),
m_opTrackInfo(p_opTrackInfo),
m_opTrackView(p_opTrackView)
{
}

CommandAddGMIDIClip::~CommandAddGMIDIClip()
{
}

EMCommand* CommandAddGMIDIClip::CloneAndClear()
{
	CommandAddGMIDIClip* opCommand = new CommandAddGMIDIClip(m_opTrackView, m_opTrackInfo, m_opClip);
	return opCommand;
}

void* CommandAddGMIDIClip::ExecuteE(void* p_upMediaClipID, void* p_upTrackIndex, void* p_upTime)
{
	bool vIsLocked(false); // FIXME: this temporary lock must be designed around
	if(m_opTrackView -> GetView() -> WindowIsLocked())
		vIsLocked = true;
	else
		m_opTrackView -> GetView() -> LockWindow();

	EMGTrack* opTrack = NULL;
	int32 vMediaClipID = *(static_cast<int32*>(p_upMediaClipID));
	uint32 vIndex = static_cast<uint32>(*(static_cast<int32*>(p_upTrackIndex)));
	int64 vDroppedAtTime = *(static_cast<int64*>(p_upTime));
	opTrack = EMGTrackRepository::Instance() -> GetTrack(vIndex);
	EMMediaClipInfo* opClipInfo = static_cast<EMMediaClipInfo*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_INFO, &vMediaClipID));
	int32 vMediaEntryID = opClipInfo -> m_vMediaEntryID;
	int64 vDuration = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_DURATION, &vMediaClipID, NULL, NULL)));
//	string oFile(static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_FILENAME, &vMediaEntryID, NULL, NULL)));
//	oFile = oFile.substr(oFile.find_last_of('/') + 1, oFile.length() - 1);
	m_opClip = EMGClipRepository::Instance() -> CreateClip(string(DEFAULT_MIDI_CLIP_NAME), EM_TYPE_MIDI, vDroppedAtTime, vDuration, opTrack, vMediaClipID, m_opTrackView -> GetView(), false);
	m_opTrackView -> GetView() -> Invalidate(m_opClip -> Frame());
	char vOutput[80];
	sprintf(vOutput, "MIDI clip created: Track: %ld, Offset: %ld", vIndex, 0L);
	TrackWindowStatusBar::Instance() -> WriteMessage(string(vOutput));
	
	if(!vIsLocked)
		m_opTrackView -> GetView() -> UnlockWindow();

	return m_opClip;
}

const char* CommandAddGMIDIClip::GetShortDescription()
{
	return "Add MIDI Clip";
}

bool CommandAddGMIDIClip::IsUndoable()
{
	return true;
}

bool CommandAddGMIDIClip::RequiresParameters()
{
	return true;
}

void CommandAddGMIDIClip::UndoE()
{
	if(m_opTrackView -> GetView() -> LockWindow())
	{
		list<EMGClip*> oClipList;
		oClipList.push_back(m_opClip);
		EMCommandRepository::Instance() -> ExecuteCommandNoUndo(COMMAND_DELETE_CLIPS_FROM_LIST, &oClipList);
		delete EMCommandRepository::Instance() -> GetUndoClone(COMMAND_DELETE_CLIPS_FROM_LIST);
		m_opTrackView -> GetView() -> UnlockWindow();
	}
}
