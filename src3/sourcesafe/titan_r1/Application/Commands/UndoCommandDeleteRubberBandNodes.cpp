#include "UndoCommandDeleteRubberBandNodes.h"

//#include "EMGRubberBand.h"
#include "EMMessages.h"
#include "EMView.h"
#include "TrackView.h"

UndoCommandDeleteRubberBandNodes::UndoCommandDeleteRubberBandNodes(TrackView* p_opTrackView) :
m_opTrackView(p_opTrackView)
{
}

UndoCommandDeleteRubberBandNodes::UndoCommandDeleteRubberBandNodes(TrackView* p_opTrackView, EMGRubberBand* p_opRubberBand, list<EMGRubberBandNode*>* p_opDeletedNodes) :
m_oDeletedNodes(*p_opDeletedNodes),
m_opRubberBand(p_opRubberBand),
m_opTrackView(p_opTrackView)
{
}

UndoCommandDeleteRubberBandNodes::~UndoCommandDeleteRubberBandNodes()
{
}

EMCommand* UndoCommandDeleteRubberBandNodes::CloneAndClear()
{
	EMCommand* opCommand = EM_new UndoCommandDeleteRubberBandNodes(m_opTrackView, m_opRubberBand, &m_oDeletedNodes);
	m_oDeletedNodes.clear();
	return opCommand;
}

void* UndoCommandDeleteRubberBandNodes::ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree)
{
	m_opRubberBand = static_cast<EMGRubberBand*>(p_opParameterOne);
	m_oDeletedNodes = /*list<EMGRubberBandNode*>* opDeletedNodes =*/ *(static_cast<list<EMGRubberBandNode*>*>(p_opParameterTwo));
	if(p_opParameterThree != NULL)
		m_oUndoCommands = *(static_cast<list<EMCommand*>*>(p_opParameterThree));
/*
	list<EMGRubberBandNode*>::iterator oIterator = opDeletedNodes -> begin();
	while(oIterator != opDeletedNodes -> end())
	{
		m_oDeletedNodes.push_back(**oIterator);
		oIterator++;
	}
*/
	return NULL;
}

const char* UndoCommandDeleteRubberBandNodes::GetShortDescription()
{
	return "Delete RubberBand Nodes";
}

bool UndoCommandDeleteRubberBandNodes::IsUndoable()
{
	return true;
}

void UndoCommandDeleteRubberBandNodes::RedoE()
{
}

bool UndoCommandDeleteRubberBandNodes::RequiresParameters()
{
	return true;
}

void UndoCommandDeleteRubberBandNodes::UndoE()
{
	if(m_opTrackView -> GetView() -> LockWindow())
	{
		while(m_oUndoCommands.size() > 0)
		{
			m_oUndoCommands.front() -> UndoE();
			m_oUndoCommands.pop_front();
		}

		m_opRubberBand -> AddNodes(&m_oDeletedNodes);
		/*
		list<EMGRubberBandNode*> oDeletedNodes;
		list<EMGRubberBandNode>::iterator oIterator = m_oDeletedNodes.begin();
		while(oIterator != m_oDeletedNodes.end())
		{
			oDeletedNodes.push_back(&(*oIterator));
			oIterator++;
		}
		m_opRubberBand -> AddNodes(&oDeletedNodes);
		*/
		/*
		// It is assumed that the data link pointers that the nodes refer to are still valid (if they pointed to anything useful in the first place)
		list<EMGRubberBandNode>::iterator oIterator = m_oDeletedNodes.begin();
		while(oIterator != m_oDeletedNodes.end())
		{
			m_opRubberBand -> CreateNode((*oIterator).m_vPosition, (*oIterator).m_vValue, (*oIterator).m_opDataLink, false, false);
			oIterator++;
		}
		*/
		
		m_opRubberBand -> InvalidateAll();

		//m_opRubberBand -> Notify(EM_RUBBERBAND_NODES_CHANGED);
		//m_opRubberBand -> Notify(EM_RUBBERBAND_NODES_ADDED);

		m_opTrackView -> GetView() -> UnlockWindow();
	}
}
