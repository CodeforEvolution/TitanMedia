#include "UndoCommandMoveRubberBandNodes.h"

#include "EMGRubberBand.h"
#include "EMMessages.h"
#include "EMView.h"
#include "TrackView.h"

UndoCommandMoveRubberBandNodes::UndoCommandMoveRubberBandNodes(TrackView* p_opTrackView) :
m_opTrackView(p_opTrackView)
{
}

UndoCommandMoveRubberBandNodes::UndoCommandMoveRubberBandNodes(TrackView* p_opTrackView, EMGRubberBand* p_opRubberBand, list<EMGRubberBandNode*> p_oMovedNodes) :
m_oMovedNodes(p_oMovedNodes),
m_opRubberBand(p_opRubberBand),
m_opTrackView(p_opTrackView)
{
	list<EMGRubberBandNode*>::iterator oIterator = m_oMovedNodes.begin();
	while(oIterator != m_oMovedNodes.end())
	{
		m_oOriginalValues.push_back((*oIterator) -> m_vStartValue);
		m_oOriginalPositions.push_back((*oIterator) -> m_vStartPosition);
		oIterator++;
	}
}

UndoCommandMoveRubberBandNodes::~UndoCommandMoveRubberBandNodes()
{
}

EMCommand* UndoCommandMoveRubberBandNodes::CloneAndClear()
{
	return EM_new UndoCommandMoveRubberBandNodes(m_opTrackView, m_opRubberBand, m_oMovedNodes);
}

void* UndoCommandMoveRubberBandNodes::ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree)
{
	m_opRubberBand = static_cast<EMGRubberBand*>(p_opParameterOne);
	m_oMovedNodes = *(static_cast<list<EMGRubberBandNode*>*>(p_opParameterTwo));

	return NULL;
}

const char* UndoCommandMoveRubberBandNodes::GetShortDescription()
{
	return "Move RubberBand Nodes";
}

bool UndoCommandMoveRubberBandNodes::IsUndoable()
{
	return true;
}

void UndoCommandMoveRubberBandNodes::RedoE()
{
}

bool UndoCommandMoveRubberBandNodes::RequiresParameters()
{
	return true;
}

void UndoCommandMoveRubberBandNodes::UndoE()
{
	if(m_opTrackView -> GetView() -> LockWindow())
	{
		list<EMGRubberBandNode*>::iterator oNodeIterator = m_oMovedNodes.begin();
		list<int32>::iterator oValueIterator = m_oOriginalValues.begin();
		list<int64>::iterator oPositionIterator = m_oOriginalPositions.begin();
		while(oNodeIterator != m_oMovedNodes.end())
		{
			(*oNodeIterator) -> m_vValue = *oValueIterator;
			(*oNodeIterator) -> m_vPosition = *oPositionIterator;
			oNodeIterator++;
			oValueIterator++;
			oPositionIterator++;
		}

		m_opRubberBand -> InvalidateAll();

		//m_opRubberBand -> Notify(EM_RUBBERBAND_NODES_CHANGED);
		//m_opRubberBand -> Notify(EM_RUBBERBAND_VALUES_CHANGED);

		m_opTrackView -> GetView() -> UnlockWindow();
	}
}
