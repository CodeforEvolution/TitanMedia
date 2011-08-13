#include "CommandAddGAudioClip.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGClipRepository.h"
#include "EMGUIUtilities.h"
#include "EMGTrack.h"
#include "EMGTrackRepository.h"
#include "EMImageBufferRepository.h"
#include "EMView.h"
#include "MediaPoolView.h"
#include "TrackView.h"
#include "TrackWindowStatusBar.h"

#include <list>
#include <string>

const char* DEFAULT_AUDIO_CLIP_NAME = "Audio Clip";

CommandAddGAudioClip::CommandAddGAudioClip(TrackView* p_opTrackView, TrackInfo* p_opTrackInfo, EMGClip* p_opClip) :
m_opClip(p_opClip),
m_opTrackInfo(p_opTrackInfo),
m_opTrackView(p_opTrackView)
{
}

CommandAddGAudioClip::~CommandAddGAudioClip()
{
}

EMCommand* CommandAddGAudioClip::CloneAndClear()
{
	CommandAddGAudioClip* opCommand = new CommandAddGAudioClip(m_opTrackView, m_opTrackInfo, m_opClip);
	return opCommand;
}

void* CommandAddGAudioClip::ExecuteE(void* p_upMediaClipID, void* p_upTrackIndex, void* p_upTime)
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

	// The starting position is truncated to nearest valid frame
	vDroppedAtTime = EMGUIUtilities::Instance() -> TruncateSample(vDroppedAtTime);

	opTrack = EMGTrackRepository::Instance() -> GetTrack(vIndex);
	EMMediaClipInfo* opClipInfo = static_cast<EMMediaClipInfo*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_INFO, &vMediaClipID));
	int32 vMediaEntryID = opClipInfo -> m_vMediaEntryID;
	//// Just to be on the safe side, add the media file to EMImageBufferRepository
	//EMImageBufferRepository::Instance() -> AudioFileAdded(vMediaEntryID);
	int64 vDuration = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_DURATION, &vMediaClipID, NULL, NULL)));
	string oFile(static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_FILENAME, &vMediaEntryID, NULL, NULL)));
	oFile = oFile.substr(oFile.find_last_of(EM_FILE_SEPARATOR) + 1, oFile.length() - 1);
	m_opClip = EMGClipRepository::Instance() -> CreateClip(oFile/*string(DEFAULT_AUDIO_CLIP_NAME)*/, EM_TYPE_ANY_AUDIO, vDroppedAtTime, vDuration, opTrack, vMediaClipID, m_opTrackView -> GetView(), false);
	EMImageBufferRepository::Instance() -> ClipAdded(m_opClip, vMediaEntryID);
	m_opTrackView -> GetView() -> Invalidate(m_opClip -> Frame());
	char vOutput[80];
	sprintf(vOutput, "Audio clip created: Track: %ld, Offset: %ld", vIndex, 0L);
	TrackWindowStatusBar::Instance() -> WriteMessage(string(vOutput));
	
	if(!vIsLocked)
		m_opTrackView -> GetView() -> UnlockWindow();

	return m_opClip;
}

const char* CommandAddGAudioClip::GetShortDescription()
{
	return "Add Audio Clip";
}

bool CommandAddGAudioClip::IsUndoable()
{
	return true;
}

bool CommandAddGAudioClip::RequiresParameters()
{
	return true;
}

void CommandAddGAudioClip::UndoE()
{
	if(m_opTrackView -> GetView() -> LockWindow())
	{
		list<EMGClip*> oClipList;
		oClipList.push_back(m_opClip);
		EMCommandRepository::Instance() -> ExecuteCommandNoUndo(COMMAND_DELETE_CLIPS_FROM_LIST, &oClipList);
		delete EMCommandRepository::Instance() -> GetUndoClone(COMMAND_DELETE_CLIPS_FROM_LIST);
		m_opTrackView -> GetView() -> UnlockWindow();
	}
/*
	uint32 vPosition = EMGTrackRepository::Instance() -> GetTrackIndex(m_opTrack);
	if(vPosition != 0)
		vPosition--;
	EMGTrackRepository::Instance() -> DeleteTrack(m_opTrack);
	m_opTrackView -> CalculateAndUpdateAll();//CalculateTrackFramesAndUpdate(m_opTrack);
*/
}
