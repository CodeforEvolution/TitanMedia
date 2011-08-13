/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_CREATE_MIDI_EVENTS
#define __COMMAND_CREATE_MIDI_EVENTS

#include "EMCommand.h"

#include <list.h>

class EMMediaMIDIEvent;
class MIDIGClipContainer;
class TrackView;

class CommandCreateMIDIEvents : public EMCommand
{
public:
	CommandCreateMIDIEvents(TrackView* p_opTrackView);
	CommandCreateMIDIEvents(TrackView* p_opTrackView, MIDIGClipContainer* p_opClipContainer, list<EMCommand*> p_oUndoCommands);
	~CommandCreateMIDIEvents();
	EMCommand* CloneAndClear();
	void ExecuteE() {};
	void* ExecuteE(void* p_vpParameters, void* p_vpEventData, void* p_opClipContainer);
	const char* GetShortDescription();
	bool IsUndoable();
	bool RequiresParameters();
	void UndoE();

private:
	list<EMCommand*> m_oUndoCommands;
	MIDIGClipContainer* m_opClipContainer;
	list<EMMediaMIDIEvent*>* m_opEvents;
	TrackView* m_opTrackView;
};

#endif

