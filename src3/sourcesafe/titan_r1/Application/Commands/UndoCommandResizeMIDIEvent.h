/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __UNDO_COMMAND_RESIZE_MIDI_EVENT
#define __UNDO_COMMAND_RESIZE_MIDI_EVENT

#include "EMCommand.h"

class EMMediaMIDIEvent;
class MIDIGClipContainer;
class TrackView;

#include <list>

class UndoCommandResizeMIDIEvent : public EMCommand
{
public:
	UndoCommandResizeMIDIEvent(TrackView* p_opTrackView);
	UndoCommandResizeMIDIEvent(TrackView* p_opTrackView, MIDIGClipContainer* p_opClipContainer, EMMediaMIDIEvent* p_opEvent, int64 p_vOriginalStartPos, int64 p_vOriginalLength);
	EMCommand* CloneAndClear();
	void ExecuteE() { };
	void* ExecuteE(void* upParameterOne, void* upParameterTwo, void* upParameterThree);
	const char* GetShortDescription();
	bool IsUndoable();
	bool RequiresParameters();
	void UndoE();

private:
	MIDIGClipContainer* m_opClipContainer;
	EMMediaMIDIEvent* m_opEvent;
	TrackView* m_opTrackView;
	int64 m_vOriginalStartPos;
	int64 m_vOriginalLength;
};

#endif

