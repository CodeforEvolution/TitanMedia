#include "CommandAddGVideoClip.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGClipContainer.h"
#include "EMGClipRepository.h"
#include "EMGUIUtilities.h"
#include "EMGTrack.h"
#include "EMGTrackRepository.h"
#include "EMImageBufferRepository.h"
#include "EMView.h"
#include "TrackView.h"
#include "TrackWindowStatusBar.h"

#include <string>

const char* DEFAULT_VIDEO_CLIP_NAME = "Video Clip";

CommandAddGVideoClip::CommandAddGVideoClip(TrackView* p_opTrackView, TrackInfo* p_opTrackInfo, EMGClip* p_opClip) :
m_opClip(p_opClip),
m_opTrackInfo(p_opTrackInfo),
m_opTrackView(p_opTrackView)
{
}

CommandAddGVideoClip::~CommandAddGVideoClip()
{
//	delete m_opTrack;
}

EMCommand* CommandAddGVideoClip::CloneAndClear()
{
	CommandAddGVideoClip* opCommand = new CommandAddGVideoClip(m_opTrackView, m_opTrackInfo, m_opClip);
//	opCommand -> SetTrack(m_opTrack);
//	m_opTrack = NULL;
	return opCommand;
}

void* CommandAddGVideoClip::ExecuteE(void* p_upMediaClipID, void* p_upTrackIndex, void* p_upTime)
{
	EMGTrack* opTrack = NULL;
	int32 vMediaClipID = *(static_cast<int32*>(p_upMediaClipID));
	uint32 vIndex = static_cast<uint32>(*(static_cast<int32*>(p_upTrackIndex)));
	int64 vDroppedAtTime = *(static_cast<int64*>(p_upTime));

	// The starting position is truncated to nearest valid frame
	vDroppedAtTime = EMGUIUtilities::Instance() -> TruncateFrame(vDroppedAtTime);

	opTrack = EMGTrackRepository::Instance() -> GetTrack(vIndex);
	EMMediaClipInfo* opClipInfo = static_cast<EMMediaClipInfo*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_INFO, &vMediaClipID));

	// If there's the possibility of overlapping video clips, the operation is aborted
	if(!(opTrack -> GetClipContainer() -> OkToDrop(NULL, vDroppedAtTime, opClipInfo -> m_vActiveDuration)))
	return NULL;

	int32 vMediaEntryID = opClipInfo -> m_vMediaEntryID;
	//// Just to be on the safe side, add the media file to EMImageBufferRepository
	//EMImageBufferRepository::Instance() -> VideoFileAdded(vMediaEntryID);
	int64 vDuration = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_DURATION, &vMediaClipID, NULL, NULL)));
	string oFile(static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_FILENAME, &vMediaEntryID, NULL, NULL)));
	//string oFile = *opFile;
	oFile = oFile.substr(oFile.find_last_of(EM_FILE_SEPARATOR) + 1, oFile.length() - 1);
	m_opClip = EMGClipRepository::Instance() -> CreateClip(oFile/*string(DEFAULT_AUDIO_CLIP_NAME)*/, EM_TYPE_ANY_VIDEO, vDroppedAtTime, vDuration, opTrack, vMediaClipID, m_opTrackView -> GetView(), false);
	EMImageBufferRepository::Instance() -> ClipAdded(m_opClip, vMediaEntryID);
	m_opTrackView -> GetView() -> Invalidate(m_opClip -> Frame());
	char vOutput[80];
	sprintf(vOutput, "Video clip created: Track: %ld, Offset: %ld", vIndex, 0L);
	TrackWindowStatusBar::Instance() -> WriteMessage(string(vOutput));
	
	return m_opClip;
}

const char* CommandAddGVideoClip::GetShortDescription()
{
	return "Add Video Clip";
}

bool CommandAddGVideoClip::IsUndoable()
{
	return true;
}

bool CommandAddGVideoClip::RequiresParameters()
{
	return true;
}

void CommandAddGVideoClip::UndoE()
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
