#include "CommandMuteClips.h"

#include "EMGClipRepository.h"
#include "EMRect.h"
#include "EMView.h"
#include "TrackView.h"

CommandMuteClips::CommandMuteClips(TrackView* p_opTrackView, list<EMGClip*> p_oMutedClips) :
m_oMutedClips(p_oMutedClips),
m_opTrackView(p_opTrackView)
{
}

CommandMuteClips::CommandMuteClips(TrackView* p_opTrackView) :
m_opTrackView(p_opTrackView)
{
}

EMCommand* CommandMuteClips::CloneAndClear()
{
	CommandMuteClips* opCommand = new CommandMuteClips(m_opTrackView, m_oMutedClips);
	m_oMutedClips.clear();
	return opCommand;
}

void* CommandMuteClips::ExecuteE(void* p_upParameterOne, void*  p_upParameterTwo, void* p_upParameterThree)
{
	m_oMutedClips = *(static_cast<list<EMGClip*>*>(p_upParameterOne));
	EMRect oInvalidateRect;
	list<EMGClip*>::iterator oIterator = m_oMutedClips.begin();
	while(oIterator != m_oMutedClips.end())
	{
		(*oIterator) -> SetMuted(!((*oIterator) -> GetMuted()));
		oInvalidateRect = oInvalidateRect | (*oIterator) -> Frame();
		oIterator++;
	}

	m_opTrackView -> GetView() -> Invalidate(oInvalidateRect);

	return NULL;
}

const char* CommandMuteClips::GetShortDescription()
{
	return "Mute Clips";
}

bool CommandMuteClips::IsUndoable()
{
	return true;
}

bool CommandMuteClips::RequiresParameters()
{
	return true;
}

void CommandMuteClips::UndoE()
{
	if(m_opTrackView -> GetView() -> LockWindow())
	{
		EMRect oInvalidateRect;
		list<EMGClip*>::iterator oIterator = m_oMutedClips.begin();
		while(oIterator != m_oMutedClips.end())
		{
			(*oIterator) -> SetMuted(!((*oIterator) -> GetMuted()));
			oInvalidateRect = oInvalidateRect | (*oIterator) -> Frame();
			oIterator++;
		}

		m_opTrackView -> GetView() -> Invalidate(oInvalidateRect);

		m_opTrackView -> GetView() -> UnlockWindow();
	}
}

