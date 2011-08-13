#include "CommandUngroupClips.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGClipRepository.h"
#include "EMRect.h"
#include "EMView.h"
#include "TrackView.h"

CommandUngroupClips::CommandUngroupClips(TrackView* p_opTrackView, list<EMGClip*> p_oUngroupedClips, list<pair<EMGClip*, EMGClip*> > p_oUngroupedClipsProperties) :
m_oUngroupedClips(p_oUngroupedClips),
m_oUngroupedClipsProperties(p_oUngroupedClipsProperties),
m_opTrackView(p_opTrackView)
{
}

CommandUngroupClips::CommandUngroupClips(TrackView* p_opTrackView) :
m_opTrackView(p_opTrackView)
{
}

EMCommand* CommandUngroupClips::CloneAndClear()
{
	CommandUngroupClips* opCommand = new CommandUngroupClips(m_opTrackView, m_oUngroupedClips, m_oUngroupedClipsProperties);
	m_oUngroupedClips.clear();
	m_oUngroupedClipsProperties.clear();
	return opCommand;
}

void CommandUngroupClips::ExecuteE()
{
	m_oUngroupedClips = *(EMGClipRepository::Instance() -> GetSelectedClips());

	int vGroupedCounter(0);
	list<EMGClip*>::iterator oIterator = m_oUngroupedClips.begin();
	while(oIterator != m_oUngroupedClips.end())
	{
		if((*oIterator) -> GetFirstInGroup() != NULL)
			EMGClipRepository::Instance() -> RemoveClipGroup((*oIterator) -> GetFirstInGroup());

		m_oUngroupedClipsProperties.push_back(pair<EMGClip*, EMGClip*>((*oIterator) -> GetFirstInGroup(), (*oIterator) -> GetNextInGroup()));
		if((*oIterator) -> GetFirstInGroup() != NULL)
			vGroupedCounter++;
		(*oIterator) -> SetFirstInGroup(NULL);
		(*oIterator) -> SetNextInGroup(NULL);
		oIterator++;
	}

	char vString[80];
	sprintf(vString, "%d clip(s) ungrouped", vGroupedCounter);
	EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_WRITE_STATUS_BAR_MESSAGE, vString);
}

const char* CommandUngroupClips::GetShortDescription()
{
	return "Ungroup Clips";
}

bool CommandUngroupClips::IsUndoable()
{
	return true;
}

bool CommandUngroupClips::RequiresParameters()
{
	return false;
}

void CommandUngroupClips::UndoE()
{
	if(m_opTrackView -> GetView() -> LockWindow())
	{
		list<EMGClip*>::iterator oUngroupedClipsIterator = m_oUngroupedClips.begin();
		list<pair<EMGClip*, EMGClip*> >::iterator oParameterIterator = m_oUngroupedClipsProperties.begin();
		while(oUngroupedClipsIterator != m_oUngroupedClips.end())
		{
			if((*oParameterIterator).first != NULL)
				EMGClipRepository::Instance() -> AddClipGroup((*oParameterIterator).first);
			(*oUngroupedClipsIterator) -> SetFirstInGroup((*oParameterIterator).first);
			(*oUngroupedClipsIterator) -> SetNextInGroup((*oParameterIterator).second);
			oUngroupedClipsIterator++;
			oParameterIterator++;
		}

		m_opTrackView -> GetView() -> UnlockWindow();
	}
}

