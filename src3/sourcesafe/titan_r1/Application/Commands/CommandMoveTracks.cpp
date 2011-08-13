#include "CommandMoveTracks.h"

#include "EMGClipRepository.h"
#include "EMGTrack.h"
#include "EMGTrackRepository.h"
#include "TrackInfo.h"
#include "TrackWindow.h"
#include "TransitionGTrackInfo.h"
#include "VideoGTrackInfo.h"

#include <algorithm>
#include <list>
#include <vector>

CommandMoveTracks::CommandMoveTracks(TrackWindow* p_opTrackWindow)
{
	m_opTrackWindow = p_opTrackWindow;
}

CommandMoveTracks::CommandMoveTracks(TrackWindow* p_opTrackWindow, vector<EMGTrack*> p_oOriginalTrackOrder) :
m_oOriginalTrackOrder(p_oOriginalTrackOrder),
m_opTrackWindow(p_opTrackWindow)
{
}

EMCommand* CommandMoveTracks::CloneAndClear()
{
	CommandMoveTracks* opCommand = EM_new CommandMoveTracks(m_opTrackWindow, m_oOriginalTrackOrder);
	m_oOriginalTrackOrder.clear();

	return opCommand;
}

void* CommandMoveTracks::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{
	uint32 vBeforeIndex = *(static_cast<uint32*>(p_upParameterOne));
	uint32 vAfterIndex = *(static_cast<uint32*>(p_upParameterTwo));
	TrackInfo* opTrackInfo = m_opTrackWindow -> GetTrackInfo();
	TrackView* opTrackView = m_opTrackWindow -> GetTrackView();
	
	if(opTrackView -> GetView() -> LockWindow())
	{
		// Creating undo information
		m_oOriginalTrackOrder = *(EMGTrackRepository::Instance() -> GetTracks());

		// If a transition track is selected but not its parent video track or vice versa, select them as well
		vector<EMGTrack*> oSelectedTracks = EMGTrackRepository::Instance() -> GetSelectedTracks();
		vector<EMGTrack*>::iterator oIterator = oSelectedTracks.begin();
		EMGTrack* opTransitionTrack;
		while(oIterator != oSelectedTracks.end())
		{
			if((*oIterator) -> GetTrackInfo() -> GetType() == EM_TYPE_ANY_VIDEO)
			{
				opTransitionTrack = dynamic_cast<VideoGTrackInfo*>((*oIterator) -> GetTrackInfo()) -> GetTransitionTrack();
				if(opTransitionTrack != NULL)
					opTransitionTrack -> GetTrackInfo() -> SetSelected(true);
			}
			else if((*oIterator) -> GetTrackInfo() -> GetType() == EM_TYPE_TRANSITION)
				dynamic_cast<TransitionGTrackInfo*>((*oIterator) -> GetTrackInfo()) -> GetVideoTrack() -> GetTrackInfo() -> SetSelected(true);

			oIterator++;
		}		
		
		// Re-cache the selected tracks
		oSelectedTracks = EMGTrackRepository::Instance() -> GetSelectedTracks();

		EMGTrackRepository::Instance() -> MoveTracks(oSelectedTracks, vBeforeIndex, vAfterIndex);
		opTrackInfo -> CalculateTrackInfoFramesAndUpdate(NULL);
		opTrackView -> CalculateAndUpdateAll();
		EMGClipRepository::Instance() -> CalculateSelectedFrame();
		opTrackView -> GetView() -> UnlockWindow();
	}

	return NULL;
}

const char* CommandMoveTracks::GetShortDescription()
{
	return "Move Tracks";
}

bool CommandMoveTracks::IsUndoable()
{
	return true;
}

bool CommandMoveTracks::RequiresParameters()
{
	return true;
}

void CommandMoveTracks::UndoE()
{
	if(m_opTrackWindow -> GetTrackView() -> GetView() -> LockWindow())
	{
		EMGTrackRepository::Instance() -> SetTrackOrder(&m_oOriginalTrackOrder);
		m_opTrackWindow -> GetTrackInfo() -> CalculateTrackInfoFramesAndUpdate(NULL);
		m_opTrackWindow -> GetTrackView() -> CalculateAndUpdateAll();
		EMGClipRepository::Instance() -> CalculateSelectedFrame();
		m_opTrackWindow -> GetTrackView() -> GetView() -> UnlockWindow();
	}
}
