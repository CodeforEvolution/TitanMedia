#include "CommandActiveMIDITrackStepDown.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGTrack.h"
#include "EMGTrackRepository.h"
#include "EMMediaGlobals.h"
#include "EMView.h"
#include "MIDIGTrackInfo.h"
#include "TrackInfo.h"

#include <vector>

CommandActiveMIDITrackStepDown::CommandActiveMIDITrackStepDown(TrackInfo* p_opTrackInfo) :
m_opTrackInfo(p_opTrackInfo)
{
}

void CommandActiveMIDITrackStepDown::ExecuteE()
{
	if(m_opTrackInfo -> GetView() -> LockWindow())
	{
		vector<EMGTrack*>* opTracks = EMGTrackRepository::Instance() -> GetTracks();
		vector<MIDIGTrackInfo*> oMIDITrackInfos;
		vector<EMGTrack*>::iterator oIterator = opTracks -> begin();
		while(oIterator != opTracks -> end())
		{
			if((*oIterator) -> GetTrackInfo() -> GetType() == EM_TYPE_MIDI)
				oMIDITrackInfos.push_back(dynamic_cast<MIDIGTrackInfo*>((*oIterator) -> GetTrackInfo()));
			oIterator++;
		}

		if(oMIDITrackInfos.size() == 0)
		{
			m_opTrackInfo -> GetView() -> UnlockWindow();
			return;
		}

		if(oMIDITrackInfos.back() -> GetActiveTrackInfo() == oMIDITrackInfos.back())
			; // Already reached the last track

		else if(oMIDITrackInfos.front() -> GetActiveTrackInfo() == NULL) // If there's no active MIDI track
		{
			oMIDITrackInfos.front() -> SetActive(true);
			m_opTrackInfo -> GetView() -> Invalidate(oMIDITrackInfos.front() -> Frame());
		}
		
		else
			for(int vIndex = 0; vIndex < oMIDITrackInfos.size(); vIndex++)
				if(oMIDITrackInfos[vIndex] -> GetActiveTrackInfo() == oMIDITrackInfos[vIndex])
				{
					oMIDITrackInfos[vIndex] -> SetActive(false);
					oMIDITrackInfos[vIndex + 1] -> SetActive(true);
					m_opTrackInfo -> GetView() -> Invalidate(oMIDITrackInfos[vIndex] -> Frame() | oMIDITrackInfos[vIndex + 1] -> Frame());
					break;
				}

		m_opTrackInfo -> GetView() -> UnlockWindow();
	}
}

bool CommandActiveMIDITrackStepDown::IsUndoable()
{
	return false;
}

bool CommandActiveMIDITrackStepDown::RequiresParameters()
{
	return false;
}

