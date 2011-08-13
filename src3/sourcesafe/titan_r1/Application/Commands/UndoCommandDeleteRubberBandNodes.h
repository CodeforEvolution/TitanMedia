/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __UNDO_COMMAND_DELETE_RUBBERBAND_NODES
#define __UNDO_COMMAND_DELETE_RUBBERBAND_NODES

#include "EMCommand.h"
#include "EMGRubberBand.h"

#include <list>

//class EMGRubberBand;
//class EMGRubberBandNode;
class TrackView;

class UndoCommandDeleteRubberBandNodes : public EMCommand
{
public:
	UndoCommandDeleteRubberBandNodes(TrackView* p_opTrackView);
	UndoCommandDeleteRubberBandNodes(TrackView* p_opTrackView, EMGRubberBand* p_opRubberBand, list<EMGRubberBandNode*>* p_opDeletedNodes);
	~UndoCommandDeleteRubberBandNodes();
	EMCommand* CloneAndClear();
	void ExecuteE() {};
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
	const char* GetShortDescription();
	bool IsUndoable();
	void RedoE();
	bool RequiresParameters();
	void UndoE();

private:
	list<EMGRubberBandNode*> m_oDeletedNodes;
	list<EMCommand*> m_oUndoCommands;
	EMGRubberBand* m_opRubberBand;
	TrackView* m_opTrackView;
};

#endif

