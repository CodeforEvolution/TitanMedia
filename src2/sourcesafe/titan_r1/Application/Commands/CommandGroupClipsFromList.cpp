#include "CommandGroupClipsFromList.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGClipRepository.h"
#include "EMRect.h"
#include "EMView.h"
#include "TrackView.h"

CommandGroupClipsFromList::CommandGroupClipsFromList(TrackView* p_opTrackView, list<EMGClip*> p_oGroupedClips, list<pair<EMGClip*, EMGClip*> > p_oGroupedClipsProperties) :
m_oGroupedClips(p_oGroupedClips),
m_oGroupedClipsProperties(p_oGroupedClipsProperties),
m_opTrackView(p_opTrackView)
{
}

CommandGroupClipsFromList::CommandGroupClipsFromList(TrackView* p_opTrackView) :
m_opTrackView(p_opTrackView)
{
}

EMCommand* CommandGroupClipsFromList::CloneAndClear()
{
	CommandGroupClipsFromList* opCommand = new CommandGroupClipsFromList(m_opTrackView, m_oGroupedClips, m_oGroupedClipsProperties);
	m_oGroupedClips.clear();
	m_oGroupedClipsProperties.clear();
	return opCommand;
}

void* CommandGroupClipsFromList::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{
	m_oGroupedClips = *(static_cast<list<EMGClip*>*>(p_upParameterOne));

	list<EMGClip*>::iterator oIterator = m_oGroupedClips.begin();
	list<EMGClip*>::iterator oAheadIterator = m_oGroupedClips.begin();
	oAheadIterator++;
	while(oAheadIterator != m_oGroupedClips.end())
	{
		m_oGroupedClipsProperties.push_back(pair<EMGClip*, EMGClip*>((*oIterator) -> GetFirstInGroup(), (*oIterator) -> GetNextInGroup()));
		(*oIterator) -> SetFirstInGroup(m_oGroupedClips.front());
		(*oIterator) -> SetNextInGroup(*oAheadIterator);
		oIterator++;
		oAheadIterator++;
	}
	m_oGroupedClips.back() -> SetFirstInGroup(m_oGroupedClips.front());
	m_oGroupedClips.back() -> SetNextInGroup(NULL);

	EMGClipRepository::Instance() -> AddClipGroup(m_oGroupedClips.front());

	char vString[80];
	sprintf(vString, "%d clip(s) grouped", m_oGroupedClips.size());
	EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_WRITE_STATUS_BAR_MESSAGE, vString);

	return NULL;
}

const char* CommandGroupClipsFromList::GetShortDescription()
{
	return "Group Clips From List";
}

bool CommandGroupClipsFromList::IsUndoable()
{
	return true;
}

bool CommandGroupClipsFromList::RequiresParameters()
{
	return true;
}

void CommandGroupClipsFromList::UndoE()
{
	if(m_opTrackView -> GetView() -> LockWindow())
	{
		list<EMGClip*>::iterator oGroupedClipsIterator = m_oGroupedClips.begin();
		list<pair<EMGClip*, EMGClip*> >::iterator oParameterIterator = m_oGroupedClipsProperties.begin();
		while(oGroupedClipsIterator != m_oGroupedClips.end())
		{
			(*oGroupedClipsIterator) -> SetFirstInGroup((*oParameterIterator).first);
			(*oGroupedClipsIterator) -> SetNextInGroup((*oParameterIterator).second);
			oGroupedClipsIterator++;
			oParameterIterator++;
		}

		m_opTrackView -> GetView() -> UnlockWindow();
	}
}

