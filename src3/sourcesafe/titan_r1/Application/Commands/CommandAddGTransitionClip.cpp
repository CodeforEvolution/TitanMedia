#include "CommandAddGTransitionClip.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGClipRepository.h"
#include "EMGTrack.h"
#include "EMGTrackRepository.h"
#include "EMView.h"
#include "TrackView.h"
#include "TrackWindowStatusBar.h"

#include <string>

const char* DEFAULT_TRANSITION_CLIP_NAME = "Transition Effect";

CommandAddGTransitionClip::CommandAddGTransitionClip(TrackView* p_opTrackView, TrackInfo* p_opTrackInfo, EMGClip* p_opClip) :
m_opClip(p_opClip),
m_opTrackInfo(p_opTrackInfo),
m_opTrackView(p_opTrackView)
{
}

CommandAddGTransitionClip::~CommandAddGTransitionClip()
{
}

EMCommand* CommandAddGTransitionClip::CloneAndClear()
{
	CommandAddGTransitionClip* opCommand = new CommandAddGTransitionClip(m_opTrackView, m_opTrackInfo, m_opClip);
	return opCommand;
}

void* CommandAddGTransitionClip::ExecuteE(void* p_upMediaClipID, void* p_upTrackIndex, void* p_upTime)
{
	EMGTrack* opTrack = NULL;
	int32 vMediaClipID = *(static_cast<int32*>(p_upMediaClipID));
	uint32 vIndex = static_cast<uint32>(*(static_cast<int32*>(p_upTrackIndex)));
	int64 vDroppedAtTime = *(static_cast<int64*>(p_upTime));
	opTrack = EMGTrackRepository::Instance() -> GetTrack(vIndex);
	EMMediaClipInfo* opClipInfo = static_cast<EMMediaClipInfo*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_INFO, &vMediaClipID));
	int32 vMediaEntryID = opClipInfo -> m_vMediaEntryID;
	int64 vDuration = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_DURATION, &vMediaClipID, NULL, NULL)));
	m_opClip = EMGClipRepository::Instance() -> CreateClip(string(DEFAULT_TRANSITION_CLIP_NAME), EM_TYPE_TRANSITION, vDroppedAtTime, vDuration, opTrack, vMediaClipID, m_opTrackView -> GetView(), false);
	m_opTrackView -> GetView() -> Invalidate(m_opClip -> Frame());
	char vOutput[80];
	sprintf(vOutput, "Transition clip created: Track: %ld, Offset: %ld", vIndex, 0L);
	TrackWindowStatusBar::Instance() -> WriteMessage(string(vOutput));
	
	return m_opClip;
}

const char* CommandAddGTransitionClip::GetShortDescription()
{
	return "Add Transition Clip";
}

bool CommandAddGTransitionClip::IsUndoable()
{
	return true;
}

bool CommandAddGTransitionClip::RequiresParameters()
{
	return true;
}

void CommandAddGTransitionClip::UndoE()
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
