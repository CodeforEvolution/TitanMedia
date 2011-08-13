/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_DELETE_MIDI_EVENTS
#define __COMMAND_DELETE_MIDI_EVENTS

#include "EMCommand.h"

#include <list>

class EMMediaMIDIEvent;
class MIDIGClipContainer;
class TrackView;

class CommandDeleteMIDIEvents : public EMCommand
{
public:
	CommandDeleteMIDIEvents(TrackView* p_opTrackView);
	CommandDeleteMIDIEvents(TrackView* p_opTrackView, MIDIGClipContainer* p_opClipContainer, list<EMCommand*> p_oUndoCommands, int32 p_vMediaTrackID);
	~CommandDeleteMIDIEvents();
	EMCommand* CloneAndClear();
	void ExecuteE() {};
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
	const char* GetShortDescription();
	bool IsUndoable();
	bool RequiresParameters();
	void UndoE();

private:
	list<EMCommand*> m_oUndoCommands;
	MIDIGClipContainer* m_opClipContainer;
	TrackView* m_opTrackView;
	int32 m_vMediaTrackID;
};

#endif

