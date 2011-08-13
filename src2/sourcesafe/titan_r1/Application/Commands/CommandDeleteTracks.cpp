#include "CommandDeleteTracks.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGClipRepository.h"
#include "EMGTrackRepository.h"
#include "EMRect.h"
#include "EMView.h"
#include "TrackView.h"

CommandDeleteTracks::CommandDeleteTracks(TrackView* p_opTrackView) :
m_opTrackView(p_opTrackView)
{
}

void CommandDeleteTracks::ExecuteE()
{
//	if(m_opTrackView -> GetView() -> LockWindow())
	{
		m_oTracks = EMGTrackRepository::Instance() -> GetSelectedTracks();
		EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DELETE_TRACKS_FROM_LIST, &m_oTracks);
/*
		list<EMGClip*> oClips;
		list<EMGClip*> oTrackClips;
		m_oTracks = EMGTrackRepository::Instance() -> GetSelectedTracks();
		if(m_oTracks.size() == 0)
		{
			m_opTrackView -> GetView() -> UnlockWindow();
			return;
		}
		EMGTrack* opTrack;
		for(uint32 vIndex = 0; vIndex < m_oTracks.size(); vIndex++)
		{
			opTrack = m_oTracks[vIndex];
			oTrackClips = *(opTrack -> GetClipContainer() -> GetClips());
			oClips.merge(oTrackClips);
		}
		list<EMGClip*>::iterator oIterator = oClips.begin();
		int32 vMediaClipID;
		EMGClipRepository::Instance() -> DeleteClips(&oClips, false);
		while(oIterator != oClips.end())
		{
			vMediaClipID = (*oIterator) -> GetMediaClipID();
			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_DELETE_CLIP, &vMediaClipID);
			oIterator++;
		}
		EMGTrackRepository::Instance() -> DeleteTracks(&m_oTracks);
		EMGClipRepository::Instance() -> CalculateSelectedFrame();
*/
//		m_opTrackView -> GetView() -> UnlockWindow();
	}
}

bool CommandDeleteTracks::IsUndoable()
{
	return false;
}

bool CommandDeleteTracks::RequiresParameters()
{
	return false;
}

