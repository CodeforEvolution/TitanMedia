#include "UndoCommandAddRubberBandNodes.h"

#include "EMGRubberBand.h"
#include "EMView.h"
#include "TrackView.h"

UndoCommandAddRubberBandNodes::UndoCommandAddRubberBandNodes(TrackView* p_opTrackView) :
m_opTrackView(p_opTrackView)
{
}

UndoCommandAddRubberBandNodes::UndoCommandAddRubberBandNodes(TrackView* p_opTrackView, EMGRubberBand* p_opRubberBand, list<EMGRubberBandNode*> p_oAddedNodes) :
m_oAddedNodes(p_oAddedNodes),
m_opRubberBand(p_opRubberBand),
m_opTrackView(p_opTrackView)
{
}

UndoCommandAddRubberBandNodes::~UndoCommandAddRubberBandNodes()
{
}

EMCommand* UndoCommandAddRubberBandNodes::CloneAndClear()
{
	EMCommand* opCommand = EM_new UndoCommandAddRubberBandNodes(m_opTrackView, m_opRubberBand, m_oAddedNodes);
	m_oAddedNodes.clear();
	return opCommand;
}

void* UndoCommandAddRubberBandNodes::ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree)
{
	m_opRubberBand = static_cast<EMGRubberBand*>(p_opParameterOne);
	m_oAddedNodes = *(static_cast<list<EMGRubberBandNode*>*>(p_opParameterTwo));
	if(p_opParameterThree != NULL)
		m_oUndoCommands = *(static_cast<list<EMCommand*>*>(p_opParameterThree));

	return NULL;
}

const char* UndoCommandAddRubberBandNodes::GetShortDescription()
{
	return "Add RubberBand Nodes";
}

bool UndoCommandAddRubberBandNodes::IsUndoable()
{
	return true;
}

void UndoCommandAddRubberBandNodes::RedoE()
{
}

bool UndoCommandAddRubberBandNodes::RequiresParameters()
{
	return true;
}

void UndoCommandAddRubberBandNodes::UndoE()
{
	if(m_opTrackView -> GetView() -> LockWindow())
	{
		m_opRubberBand -> DeleteNodes(&m_oAddedNodes, false, false);
		m_opRubberBand -> InvalidateAll();

		while(m_oUndoCommands.size() > 0)
		{
			m_oUndoCommands.front() -> UndoE();
			m_oUndoCommands.pop_front();
		}

		m_opTrackView -> GetView() -> UnlockWindow();
	}
}
