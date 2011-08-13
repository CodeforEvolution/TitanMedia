/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_AUDIO_DESTRUCTIVE_FX
#define __COMMAND_AUDIO_DESTRUCTIVE_FX

#include "DestructiveEditBase.h"
#include "EMCommand.h"

#include <list>

class AudioGClip;
class TrackView;

class CommandAudioDestructiveFX : public EMCommand, public DestructiveEditBase
{
public:
	CommandAudioDestructiveFX(TrackView* p_opTrackView);
	CommandAudioDestructiveFX(TrackView* p_opTrackView, list<AudioGClip*> p_oAffectedClips, list<AudioGClip*> p_oNewClips, list<EMCommand*> p_oUndoCommands);
	EMCommand* CloneAndClear();
	void ExecuteE() {};
	void* ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree);
	const char* GetShortDescription();
	bool IsUndoable();
	bool RequiresParameters();
	void UndoE();
};

#endif
