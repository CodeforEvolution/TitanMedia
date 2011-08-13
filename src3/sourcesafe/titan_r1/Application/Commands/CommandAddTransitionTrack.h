/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_ADD_TRANSITION_TRACK
#define __COMMAND_ADD_TRANSITION_TRACK

#include "EMCommand.h"

class EMGTrack;
class TrackInfo;
class TrackView;

class CommandAddTransitionTrack : public EMCommand
{
public:
	CommandAddTransitionTrack(TrackView* p_opTrackView, TrackInfo* p_opTrackInfo);
	~CommandAddTransitionTrack();
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();
	void SetTrack(EMGTrack* p_opTrack);

private:
	EMGTrack* m_opTrack;
	TrackInfo* m_opTrackInfo;
	TrackView* m_opTrackView;
};

#endif

