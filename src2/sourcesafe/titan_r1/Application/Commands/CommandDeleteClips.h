/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_DELETE_CLIPS
#define __COMMAND_DELETE_CLIPS

#include "EMCommand.h"
#include "EMGClip.h"

#include <list>

class TrackView;

class CommandDeleteClips : public EMCommand
{
public:
	CommandDeleteClips();//TrackView* p_opTrackView);
	EMCommand* CloneAndClear();
	void ExecuteE();
	void* ExecuteE(void* , void* , void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();
};

#endif
