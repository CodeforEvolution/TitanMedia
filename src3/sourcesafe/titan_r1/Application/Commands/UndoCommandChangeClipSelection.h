/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __UNDO_COMMAND_CHANGE_CLIP_SELECTION
#define __UNDO_COMMAND_CHANGE_CLIP_SELECTION

#include "EMCommand.h"
#include "EMGClip.h"

class EMGClip;
class TrackView;

#include <list>

class UndoCommandChangeClipSelection : public EMCommand
{
public:
	UndoCommandChangeClipSelection(TrackView* p_opTrackView, list<EMGClip*>* p_opClipsToBeSelected);
	EMCommand* CloneAndClear();
	void ExecuteE() { };
	void* ExecuteE(void* upParameterOne, void* upParameterTwo, void* upParameterThree);
	const char* GetShortDescription();
	bool IsUndoable();
	bool RequiresParameters();
	void UndoE();

private:
	list<EMGClip*> m_oClipsToBeSelected;
	TrackView* m_opTrackView;
};

#endif

