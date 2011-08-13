/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_MIDI_QUICK_QUANTIZE
#define __COMMAND_MIDI_QUICK_QUANTIZE

#include "EMCommand.h"

#include <list>

class EMGClip;
class TrackWindow;

class CommandMIDIQuickQuantize : public EMCommand
{
public:
	CommandMIDIQuickQuantize(TrackWindow* p_opTrackWindow);
	CommandMIDIQuickQuantize(TrackWindow* p_opTrackWindow, list<EMGClip*> p_oAffectedClips, EMCommand* p_opMediaUndoCommand);
	EMCommand* CloneAndClear();
	void ExecuteE();
	void* ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree) { return NULL; }
	bool IsUndoable();
	bool RequiresParameters();
	void UndoE();

private:
	list<EMGClip*> m_oAffectedClips;
	EMCommand* m_opMediaUndoCommand;
	TrackWindow* m_opTrackWindow;
};

#endif
