/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_AUDIO_NORMALIZE
#define __COMMAND_AUDIO_NORMALIZE

#include "DestructiveEditBase.h"
#include "EMCommand.h"

#include <list>

class AudioGClip;
class TrackView;

class CommandAudioNormalize : public EMCommand, public DestructiveEditBase
{
public:
	CommandAudioNormalize(TrackView* p_opTrackView);
	CommandAudioNormalize(TrackView* p_opTrackView, list<AudioGClip*> p_oAffectedClips, list<AudioGClip*> p_oNewClips, list<EMCommand*> p_oUndoCommands);
	EMCommand* CloneAndClear();
	void DialogCallback(int32 p_vDialogParameter);
	void ExecuteE();
	void* ExecuteE(void*, void*, void*) { return NULL; };
	const char* GetShortDescription();
	bool IsUndoable();
	bool RequiresParameters();
	void UndoE();
};

#endif
