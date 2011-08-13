/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_AUDIO_LOCK_TO_TEMPO
#define __COMMAND_AUDIO_LOCK_TO_TEMPO

#include "EMCommand.h"

class EMMenuBar;

class CommandAudioLockToTempo : public EMCommand
{
public:
	CommandAudioLockToTempo();
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();
};

#endif
