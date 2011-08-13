/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __UNDO_COMMAND_CHANGE_CLIP_RESIZE
#define __UNDO_COMMAND_CHANGE_CLIP_RESIZE

#include "EMCommand.h"
#include "EMGClip.h"
#include "EMRect.h"

class EMGClip;
class TrackView;

#include <list>

class UndoCommandChangeClipResize : public EMCommand
{
public:
	UndoCommandChangeClipResize(TrackView* p_opTrackView, EMGClip* p_opClip, list<int64>* p_opClipParameters, EMRect p_oClipFrame);
	EMCommand* CloneAndClear();
	void ExecuteE() { };
	void* ExecuteE(void* upParameterOne, void* upParameterTwo, void* upParameterThree);
	const char* GetShortDescription();
	bool IsUndoable();
	bool RequiresParameters();
	void UndoE();

private:
	EMRect m_oClipFrame;
	EMGClip* m_opClip;
	list<int64> m_oClipParameters;
	TrackView* m_opTrackView;
};

#endif

