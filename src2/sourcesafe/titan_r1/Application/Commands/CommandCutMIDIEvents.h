/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_CUT_MIDI_EVENTS
#define __COMMAND_CUT_MIDI_EVENTS

#include "EMCommand.h"

#include <list>

class EMMediaMIDIEvent;
class MIDIGClipContainer;
class TrackView;

class CommandCutMIDIEvents : public EMCommand
{
public:
	CommandCutMIDIEvents(TrackView* p_opTrackView);
	CommandCutMIDIEvents(TrackView* p_opTrackView, MIDIGClipContainer* p_opClipContainer, list<EMMediaMIDIEvent*> p_oCutEvents, int32 p_vMediaTrackID, int64 p_vCutPos, list<EMCommand*> m_oUndoCommands);
	~CommandCutMIDIEvents();
	EMCommand* CloneAndClear();
	void ExecuteE() {};
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
	const char* GetShortDescription();
	bool IsUndoable();
	bool RequiresParameters();
	void UndoE();

private:
	list<EMMediaMIDIEvent*> m_oCutEvents; // Pointers can be used here since the events are not really deleted in the media engine
	MIDIGClipContainer* m_opClipContainer;
	TrackView* m_opTrackView;
	list<EMCommand*> m_oUndoCommands;
	int64 m_vCutPos;
	int32 m_vMediaTrackID;
};

#endif

