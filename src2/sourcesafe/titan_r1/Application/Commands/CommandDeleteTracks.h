/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_DELETE_TRACKS
#define __COMMAND_DELETE_TRACKS

#include "EMCommand.h"
#include "EMGTrack.h"

#include <vector>

class TrackView;

class CommandDeleteTracks : public EMCommand
{
public:
	CommandDeleteTracks(TrackView* p_opTrackView);
	void ExecuteE();
	void* ExecuteE(void* , void* , void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();

private:
	TrackView* m_opTrackView;
	vector<EMGTrack*> m_oTracks;
};

#endif
