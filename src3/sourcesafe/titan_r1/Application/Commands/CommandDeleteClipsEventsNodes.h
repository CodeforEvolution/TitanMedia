/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_DELETE_CLIPS_EVENTS_NODES
#define __COMMAND_DELETE_CLIPS_EVENTS_NODES

#include "EMCommand.h"

class TrackView;

class CommandDeleteClipsEventsNodes : public EMCommand
{
public:
	CommandDeleteClipsEventsNodes(TrackView* p_opTrackView);
	~CommandDeleteClipsEventsNodes();
	void* ExecuteE(void*, void*, void*) { return NULL; };
	void ExecuteE();
	const char* GetShortDescription();
	bool IsUndoable();
	bool RequiresParameters();

private:
	TrackView* m_opTrackView;
};

#endif

