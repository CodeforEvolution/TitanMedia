/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __UNDO_COMMAND_ADD_RUBBERBAND_NODES
#define __UNDO_COMMAND_ADD_RUBBERBAND_NODES

#include "EMCommand.h"

#include <list>

class EMGRubberBand;
class EMGRubberBandNode;
class TrackView;

class UndoCommandAddRubberBandNodes : public EMCommand
{
public:
	UndoCommandAddRubberBandNodes(TrackView* p_opTrackView);
	UndoCommandAddRubberBandNodes(TrackView* p_opTrackView, EMGRubberBand* p_opRubberBand, list<EMGRubberBandNode*> p_oAddedNodes);
	~UndoCommandAddRubberBandNodes();
	EMCommand* CloneAndClear();
	void ExecuteE() {};
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
	const char* GetShortDescription();
	bool IsUndoable();
	void RedoE();
	bool RequiresParameters();
	void UndoE();

private:
	list<EMGRubberBandNode*> m_oAddedNodes;
	list<EMCommand*> m_oUndoCommands;
	EMGRubberBand* m_opRubberBand;
	TrackView* m_opTrackView;
};

#endif

