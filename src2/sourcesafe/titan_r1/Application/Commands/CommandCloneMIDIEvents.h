/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_CLONE_MIDI_EVENTS
#define __COMMAND_CLONE_MIDI_EVENTS

#include "EMCommand.h"

#include <list>

class EMMediaMIDIEvent;
class MIDIGClipContainer;
class TrackView;

class CommandCloneMIDIEvents : public EMCommand
{
public:
	CommandCloneMIDIEvents(TrackView* p_opTrackView);
	CommandCloneMIDIEvents(TrackView* p_opTrackView, MIDIGClipContainer* p_opClipContainer, EMCommand* p_opMediaUndoCommand);
	~CommandCloneMIDIEvents();
	EMCommand* CloneAndClear();
	void ExecuteE() {};
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
	const char* GetShortDescription();
	bool IsUndoable();
	bool RequiresParameters();
	void UndoE();

private:
	list<EMMediaMIDIEvent*> m_oClonedEvents; // Used only to be able to return cloned events
	EMCommand* m_opMediaUndoCommand;
	MIDIGClipContainer* m_opClipContainer;
	TrackView* m_opTrackView;
};

#endif

