/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_AUDIO_REVERSE
#define __COMMAND_AUDIO_REVERSE

#include "DestructiveEditBase.h"
#include "EMCommand.h"

#include <list>

class AudioGClip;
class TrackView;

class CommandAudioReverse : public EMCommand, public DestructiveEditBase
{
public:
	CommandAudioReverse(TrackView* p_opTrackView);
	CommandAudioReverse(TrackView* p_opTrackView, list<AudioGClip*> p_oAffectedClips, list<AudioGClip*> p_oNewClips, list<EMCommand*> p_oUndoCommands);
	EMCommand* CloneAndClear();
	void ExecuteE();
	void* ExecuteE(void*, void*, void*) { return NULL; };
	const char* GetShortDescription();
	bool IsUndoable();
	bool RequiresParameters();
	void UndoE();
};

#endif
