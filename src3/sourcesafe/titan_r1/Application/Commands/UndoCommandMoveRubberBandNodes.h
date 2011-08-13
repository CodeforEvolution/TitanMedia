/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __UNDO_COMMAND_MOVE_RUBBERBAND_NODES
#define __UNDO_COMMAND_MOVE_RUBBERBAND_NODES

#include "EMCommand.h"

#include <list>

class EMGRubberBand;
class EMGRubberBandNode;
class TrackView;

class UndoCommandMoveRubberBandNodes : public EMCommand
{
public:
	UndoCommandMoveRubberBandNodes(TrackView* p_opTrackView);
	UndoCommandMoveRubberBandNodes(TrackView* p_opTrackView, EMGRubberBand* p_opRubberBand, list<EMGRubberBandNode*> p_oMovedNodes);
	~UndoCommandMoveRubberBandNodes();
	EMCommand* CloneAndClear();
	void ExecuteE() {};
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
	const char* GetShortDescription();
	bool IsUndoable();
	void RedoE();
	bool RequiresParameters();
	void UndoE();

private:
	list<int64> m_oOriginalPositions;
	list<int32> m_oOriginalValues;
	list<EMGRubberBandNode*> m_oMovedNodes;
	EMGRubberBand* m_opRubberBand;
	TrackView* m_opTrackView;
};

#endif

