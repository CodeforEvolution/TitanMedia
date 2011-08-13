#include "CommandDeleteTracksFromList.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGClipRepository.h"
#include "EMGTrackRepository.h"
#include "EMRect.h"
#include "EMView.h"
#include "TrackView.h"
#include "TransitionGTrackInfo.h"
#include "VideoGTrackInfo.h"

#include <algorithm>

CommandDeleteTracksFromList::CommandDeleteTracksFromList(TrackView* p_opTrackView) :
m_opTrackView(p_opTrackView),
m_opUndoedClipDeletion(NULL)
{
}

CommandDeleteTracksFromList::CommandDeleteTracksFromList(TrackView* p_opTrackView, EMCommand* p_opUndoedClipDeletion, vector<EMGTrack*> p_oTracks, list<EMCommand*> p_oUndoedMediaTrackDeletions) :
m_oUndoedMediaTrackDeletions(p_oUndoedMediaTrackDeletions),
m_opTrackView(p_opTrackView),
m_opUndoedClipDeletion(p_opUndoedClipDeletion),
m_oTracks(p_oTracks)
{
}

EMCommand* CommandDeleteTracksFromList::CloneAndClear()
{
	CommandDeleteTracksFromList* opCommand = new CommandDeleteTracksFromList(m_opTrackView, m_opUndoedClipDeletion, m_oTracks, m_oUndoedMediaTrackDeletions);
	m_oUndoedMediaTrackDeletions.clear();
	m_oTracks.clear();
	return opCommand;
}

void* CommandDeleteTracksFromList::ExecuteE(void* p_vParameterOne, void* p_vParameterTwo, void* p_vParameterThree)
{
	if(m_opTrackView -> GetView() -> LockWindow())
	{
		list<EMGClip*> oClips;
		list<EMGClip*> oTrackClips;
		vector<EMGTrack*> oTracks = *(static_cast<vector<EMGTrack*>*>(p_vParameterOne));
		if(oTracks.size() == 0)
		{
			m_opTrackView -> GetView() -> UnlockWindow();
			return NULL;
		}
		EMGTrack* opTrack;

		// Add transition tracks whose parent video tracks are to be deleted
		list<EMGTrack*> oTransitionTracks;
		for(uint32 vIndex = 0; vIndex < oTracks.size(); vIndex++)
		{
			opTrack = oTracks[vIndex];

			if(opTrack -> GetTrackInfo() -> GetType() == EM_TYPE_ANY_VIDEO)
			{
				VideoGTrackInfo* opInfo = dynamic_cast<VideoGTrackInfo*>(opTrack -> GetTrackInfo());
				if(opInfo -> GetTransitionTrack() != NULL)
					oTransitionTracks.push_back(opInfo -> GetTransitionTrack());
			}
		}
		while(oTransitionTracks.size() > 0)
		{
			if(find(oTracks.begin(), oTracks.end(), oTransitionTracks.front()) == oTracks.end()) // If not already amongst the tracks to be deleted
				oTracks.push_back(oTransitionTracks.front());
			oTransitionTracks.pop_front();
		}

		// Disconnect deleted transition tracks from their parent video tracks
		for(vIndex = 0; vIndex < oTracks.size(); vIndex++)
		{
			opTrack = oTracks[vIndex];

			if(opTrack -> GetTrackInfo() -> GetType() == EM_TYPE_TRANSITION)
			{
				TransitionGTrackInfo* opInfo = dynamic_cast<TransitionGTrackInfo*>(opTrack -> GetTrackInfo());
					dynamic_cast<VideoGTrackInfo*>(opInfo -> GetVideoTrack() -> GetTrackInfo()) -> SetTransitionTrack(NULL);
			}
		}

		// Make a list of all clips to delete
		for(vIndex = 0; vIndex < oTracks.size(); vIndex++)
		{
			opTrack = oTracks[vIndex];

			oTrackClips = *(opTrack -> GetClipContainer() -> GetClips());
			oClips.merge(oTrackClips);
		}

		// Get the original track state to be restored if the command is undoed
		m_oTracks = *(EMGTrackRepository::Instance() -> GetTracks());
			
		if(oClips.size() > 0)
		{
			EMCommandRepository::Instance() -> ExecuteCommandNoUndo(COMMAND_DELETE_CLIPS_FROM_LIST, &oClips);
			m_opUndoedClipDeletion = EMCommandRepository::Instance() -> GetUndoClone(COMMAND_DELETE_CLIPS_FROM_LIST);
		}

		EMGTrackRepository::Instance() -> DeleteTracks(&oTracks, false);

		EMGClipRepository::Instance() -> CalculateSelectedFrame();

		int32 vMediaTrackID;
		for(vIndex = 0; vIndex < oTracks.size(); vIndex++)
		{
			if(EMGClipContainer::GetClipContainerWithSelection() == oTracks[vIndex] -> GetClipContainer())
				oTracks[vIndex] -> GetClipContainer() -> DeselectAll(NULL); // If it contains a selection, remove it
			vMediaTrackID = oTracks[vIndex] -> GetTrackInfo() -> GetMediaTrackID();
			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_DELETE_TRACK, &vMediaTrackID);
			m_oUndoedMediaTrackDeletions.push_back(EMCommandRepository::Instance() -> GetUndoClone(MEDIA_COMMAND_DELETE_TRACK));
		}
	
		m_opTrackView -> GetView() -> UnlockWindow();
	}

	return NULL;
}

const char* CommandDeleteTracksFromList::GetShortDescription()
{
	return "Delete Tracks";
}

bool CommandDeleteTracksFromList::IsUndoable()
{
	return true;
}

bool CommandDeleteTracksFromList::RequiresParameters()
{
	return true;
}

// One nasty UndoE mother
void CommandDeleteTracksFromList::UndoE()
{
	if(m_opTrackView -> GetView() -> LockWindow())
	{
		// First, bring back the media tracks
		EMCommand* opCommand;
		list<EMCommand*>::iterator oMediaTrackCommandIterator = m_oUndoedMediaTrackDeletions.begin();
		while(oMediaTrackCommandIterator != m_oUndoedMediaTrackDeletions.end())
		{
			opCommand = *oMediaTrackCommandIterator;
			opCommand -> UndoE();
			oMediaTrackCommandIterator++;
			delete opCommand;
		}

		// Restore all EMGTracks
		EMGTrackRepository::Instance() -> SetTracks(&m_oTracks);

		// Restore the deleted clips if any
		if(m_opUndoedClipDeletion != NULL)
			m_opUndoedClipDeletion -> UndoE();

		m_opTrackView -> GetView() -> UnlockWindow();
	}
}

