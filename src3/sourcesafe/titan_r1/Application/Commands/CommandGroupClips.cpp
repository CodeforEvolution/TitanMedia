#include "CommandGroupClips.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGClipRepository.h"
//#include "EMRect.h"
//#include "EMView.h"
//#include "TrackView.h"

CommandGroupClips::CommandGroupClips(TrackView* p_opTrackView, list<EMGClip*> p_oGroupedClips, list<pair<EMGClip*, EMGClip*> > p_oGroupedClipsProperties) :
m_oGroupedClips(p_oGroupedClips),
m_oGroupedClipsProperties(p_oGroupedClipsProperties),
m_opTrackView(p_opTrackView)
{
}

CommandGroupClips::CommandGroupClips(TrackView* p_opTrackView) :
m_opTrackView(p_opTrackView)
{
}

EMCommand* CommandGroupClips::CloneAndClear()
{
	CommandGroupClips* opCommand = new CommandGroupClips(m_opTrackView, m_oGroupedClips, m_oGroupedClipsProperties);
	m_oGroupedClips.clear();
	m_oGroupedClipsProperties.clear();
	return opCommand;
}

void CommandGroupClips::ExecuteE()
{
	m_oGroupedClips = *(EMGClipRepository::Instance() -> GetSelectedClips());//*(static_cast<list<EMGClip*>*>(p_upParameterOne));

	EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_GROUP_CLIPS_FROM_LIST, &m_oGroupedClips, NULL, NULL);

/*	list<EMGClip*>::iterator oIterator = m_oGroupedClips.begin();
	list<EMGClip*>::iterator oAheadIterator = m_oGroupedClips.begin();
	oAheadIterator++;
	while(oAheadIterator != m_oGroupedClips.end())
	{
eo << "CommandGroupClips: " << (uint32) (*oIterator) -> GetFirstInGroup() << ", " << (uint32) (*oIterator) -> GetNextInGroup() << ef;
		m_oGroupedClipsProperties.push_back(pair<EMGClip*, EMGClip*>((*oIterator) -> GetFirstInGroup(), (*oIterator) -> GetNextInGroup()));
		(*oIterator) -> SetFirstInGroup(m_oGroupedClips.front());
		(*oIterator) -> SetNextInGroup(*oAheadIterator);
		oIterator++;
		oAheadIterator++;
	}
	m_oGroupedClipsProperties.push_back(pair<EMGClip*, EMGClip*>(m_oGroupedClips.back() -> GetFirstInGroup(), m_oGroupedClips.back() -> GetNextInGroup()));
	m_oGroupedClips.back() -> SetFirstInGroup(m_oGroupedClips.front());
	m_oGroupedClips.back() -> SetNextInGroup(NULL);

	EMGClipRepository::Instance() -> AddClipGroup(m_oGroupedClips.front());
*/
}

const char* CommandGroupClips::GetShortDescription()
{
	return "Group Clips";
}

bool CommandGroupClips::IsUndoable()
{
	return false;
}

bool CommandGroupClips::RequiresParameters()
{
	return false;
}

void CommandGroupClips::UndoE()
{
/*	if(m_opTrackView -> GetView() -> LockWindow())
	{
		list<EMGClip*>::iterator oGroupedClipsIterator = m_oGroupedClips.begin();
		list<pair<EMGClip*, EMGClip*> >::iterator oParameterIterator = m_oGroupedClipsProperties.begin();
		while(oGroupedClipsIterator != m_oGroupedClips.end())
		{
eo << "CommandGroupClips (undo): " << (uint32) (*oParameterIterator).first << ", " << (uint32) (*oParameterIterator).second << ef;
			(*oGroupedClipsIterator) -> SetFirstInGroup((*oParameterIterator).first);
			(*oGroupedClipsIterator) -> SetNextInGroup((*oParameterIterator).second);
			oGroupedClipsIterator++;
			oParameterIterator++;
		}

		EMGClipRepository::Instance() -> RemoveClipGroup(m_oGroupedClips.front());

		m_opTrackView -> GetView() -> UnlockWindow();
	}
*/
}

