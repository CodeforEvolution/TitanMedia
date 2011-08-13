#include "CommandChangeVideoPlaybackSpeed.h"

#include "CommandIDs.h"
#include "EMGClip.h"
#include "EMGClipRepository.h"
#include "EMGTrack.h"
#include "EMImageBufferRepository.h"
#include "EMView.h"
#include "TrackView.h"
#include "VideoGClip.h"

CommandChangeVideoPlaybackSpeed::CommandChangeVideoPlaybackSpeed(TrackView* p_opTrackView) :
m_opTrackView(p_opTrackView)
{
}

CommandChangeVideoPlaybackSpeed::CommandChangeVideoPlaybackSpeed(TrackView* p_opTrackView, list<VideoGClip*> p_oAffectedClips, list<EMCommand*> p_oUndoCommands) :
m_opTrackView(p_opTrackView),
m_oAffectedClips(p_oAffectedClips),
m_oUndoCommands(p_oUndoCommands)
{
}

EMCommand* CommandChangeVideoPlaybackSpeed::CloneAndClear()
{
	EMCommand* opCommand = EM_new CommandChangeVideoPlaybackSpeed(m_opTrackView, m_oAffectedClips, m_oUndoCommands);
	m_oAffectedClips.clear();
	m_oUndoCommands.clear();

	return opCommand;
}

void* CommandChangeVideoPlaybackSpeed::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{
	if(m_opTrackView -> GetView() -> LockWindow())
	{
		// Build a list of affected video clips and a list of affected video tracks
		list<EMGTrack*> oTracks;
		list<EMGClip*>::iterator oSelectedClipsIterator = EMGClipRepository::Instance() -> GetSelectedClips() -> begin();
		while(oSelectedClipsIterator != EMGClipRepository::Instance() -> GetSelectedClips() -> end())
		{
			if((*oSelectedClipsIterator) -> GetType() == EM_TYPE_ANY_VIDEO)
			{
				m_oAffectedClips.push_back(static_cast<VideoGClip*>(*oSelectedClipsIterator));
				oTracks.push_back((*oSelectedClipsIterator) -> GetTrack());
			}
			oSelectedClipsIterator++;
		}
		oTracks.unique();

		uint32 vVideoClipID(0);
		list<VideoGClip*>::iterator oAffectedClipsIterator = m_oAffectedClips.begin();
		while(oAffectedClipsIterator != m_oAffectedClips.end())
		{
			vVideoClipID = (*oAffectedClipsIterator) -> GetMediaClipID();
			EMCommandRepository::Instance() -> ExecuteCommandNoUndo(MEDIA_COMMAND_SET_MAGIC_SPEED_VALUE, &vVideoClipID, p_upParameterOne, NULL); // p_upParameterOne = MagicSpeed
			m_oUndoCommands.push_back(EMCommandRepository::Instance() -> GetUndoClone(MEDIA_COMMAND_SET_MAGIC_SPEED_VALUE));
			oAffectedClipsIterator++;
		}

		// Refresh the image buffer information and update the frames on the clips
		EMImageBufferRepository* opIBR = EMImageBufferRepository::Instance();
		oAffectedClipsIterator = m_oAffectedClips.begin();
		while(oAffectedClipsIterator != m_oAffectedClips.end())
		{
			opIBR -> ClipRemoved(*oAffectedClipsIterator);
			oAffectedClipsIterator++;
		}
		oAffectedClipsIterator = m_oAffectedClips.begin();
		int32 vMediaClipID;
		int32 vEntryID;
		while(oAffectedClipsIterator != m_oAffectedClips.end())
		{
			(*oAffectedClipsIterator) -> UpdateFrame();
			vMediaClipID = (*oAffectedClipsIterator) -> GetMediaClipID();
			vEntryID = *(static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_ENTRY_ID, &vMediaClipID)));
			opIBR -> ClipAdded(*oAffectedClipsIterator, vEntryID);
			(*oAffectedClipsIterator) -> UpdateContent();
			oAffectedClipsIterator++;
		}

		// Move clips to prevent overlapping
		bool vClipsHaveMoved(false);
		EMGClip* opPreviousClip;
		list<EMGTrack*>::iterator oTrackIterator = oTracks.begin();
		list<EMGClip*>::iterator oClipIterator;
		list<EMGClip*> oClipsToMove;
		int32 vTrackOffset(0);
		int64 vPositionOffset;
		while(oTrackIterator != oTracks.end())
		{
			if((*oTrackIterator) -> GetClipContainer() -> GetClips() -> size() >= 2)
			{
				opPreviousClip = (*oTrackIterator) -> GetClipContainer() -> GetClips() -> front();
				oClipIterator = (*oTrackIterator) -> GetClipContainer() -> GetClips() -> begin();
				oClipIterator++;
				while(oClipIterator != (*oTrackIterator) -> GetClipContainer() -> GetClips() -> end())
				{
					if((*oClipIterator) -> GetPosition() <= opPreviousClip -> GetPosition() + opPreviousClip -> GetLength() - 1)
					{
						vClipsHaveMoved = true;
						oClipsToMove.clear();
						oClipsToMove.push_back(*oClipIterator);
						vPositionOffset = opPreviousClip -> GetPosition() + opPreviousClip -> GetLength() - (*oClipIterator) -> GetPosition();
						EMCommandRepository::Instance() -> ExecuteCommandNoUndo(COMMAND_MOVE_CLIPS, &vTrackOffset, &vPositionOffset, &oClipsToMove);
						m_oUndoCommands.push_back(EMCommandRepository::Instance() -> GetUndoClone(COMMAND_MOVE_CLIPS));
					}
					oClipIterator++;
				}
			}
			oTrackIterator++;
		}

		m_opTrackView -> GetView() -> Invalidate(m_opTrackView -> GetView() -> Bounds());

		m_opTrackView -> GetView() -> UnlockWindow();
	}

	return NULL;
}

const char* CommandChangeVideoPlaybackSpeed::GetShortDescription()
{
	return "Change Video Playback Speed";
}

bool CommandChangeVideoPlaybackSpeed::IsUndoable()
{
	return true;
}

bool CommandChangeVideoPlaybackSpeed::RequiresParameters()
{
	return true;
}

void CommandChangeVideoPlaybackSpeed::UndoE()
{
	if(m_opTrackView -> GetView() -> LockWindow())
	{		
		EMCommand* opCommand;
		while(m_oUndoCommands.size() > 0)
		{
			opCommand = m_oUndoCommands.back();
			m_oUndoCommands.pop_back();
			opCommand -> UndoE();
			delete opCommand;
		}

		// Refresh the image buffer information and update the frames on the clips
		EMImageBufferRepository* opIBR = EMImageBufferRepository::Instance();
		list<VideoGClip*>::iterator oAffectedClipsIterator = m_oAffectedClips.begin();
		while(oAffectedClipsIterator != m_oAffectedClips.end())
		{
			opIBR -> ClipRemoved(*oAffectedClipsIterator);
			oAffectedClipsIterator++;
		}
		oAffectedClipsIterator = m_oAffectedClips.begin();
		int32 vMediaClipID;
		int32 vEntryID;
		while(oAffectedClipsIterator != m_oAffectedClips.end())
		{
			(*oAffectedClipsIterator) -> UpdateFrame();
			vMediaClipID = (*oAffectedClipsIterator) -> GetMediaClipID();
			vEntryID = *(static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_ENTRY_ID, &vMediaClipID)));
			opIBR -> ClipAdded(*oAffectedClipsIterator, vEntryID);
			oAffectedClipsIterator++;
		}

		m_opTrackView -> GetView() -> Invalidate(m_opTrackView -> GetView() -> Bounds());

		m_opTrackView -> GetView() -> UnlockWindow();
	}
}
