/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_DELETE_CLIPS_FROM_LIST
#define __COMMAND_DELETE_CLIPS_FROM_LIST

#include "EMCommand.h"
#include "EMGClip.h"

#include <list>

class TrackView;

class CommandDeleteClipsFromList : public EMCommand
{
public:
	CommandDeleteClipsFromList(TrackView* p_opTrackView, list<EMGClip*> p_oDeletedClips, list<EMCommand*> p_oMediaCommands);
	CommandDeleteClipsFromList(TrackView* p_opTrackView);
	EMCommand* CloneAndClear();
	void ExecuteE() {};
	void* ExecuteE(void* , void* , void* );
	const char* GetShortDescription();
	bool IsUndoable();
	bool RequiresParameters();
	void UndoE();

private:
	list<EMGClip*> m_oDeletedClips;
	list<EMCommand*> m_oMediaCommands;
	TrackView* m_opTrackView;
};

#endif
