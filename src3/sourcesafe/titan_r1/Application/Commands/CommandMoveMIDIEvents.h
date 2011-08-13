/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_MOVE_MIDI_EVENTS
#define __COMMAND_MOVE_MIDI_EVENTS

#include "EMCommand.h"

#include <list>

class EMMediaMIDIEvent;
class MIDIGClipContainer;
class TrackView;

class CommandMoveMIDIEvents : public EMCommand
{
public:
	CommandMoveMIDIEvents(TrackView* p_opTrackView);
	CommandMoveMIDIEvents(TrackView* p_opTrackView, MIDIGClipContainer* p_opClipContainer, list<EMMediaMIDIEvent*> p_oMovedEvents, int32 p_vKeyOffset, int64 p_vPositionOffset);
	~CommandMoveMIDIEvents();
	EMCommand* CloneAndClear();
	void ExecuteE() {};
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
	const char* GetShortDescription();
	bool IsUndoable();
	void RedoE();
	bool RequiresParameters();
	void UndoE();

private:
	void ExecuteBody(list<EMMediaMIDIEvent*>* p_opMovedClips, int32 p_vTrackOffset, int64 p_vPositionOffset);

private:
	list<EMMediaMIDIEvent*> m_oMovedEvents;
	MIDIGClipContainer* m_opClipContainer;
	TrackView* m_opTrackView;
	int64 m_vPositionOffset;
	int32 m_vKeyOffset;
};

#endif

