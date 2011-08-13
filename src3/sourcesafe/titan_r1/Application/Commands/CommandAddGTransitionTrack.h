/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_ADD_G_TRANSITION_TRACK
#define __COMMAND_ADD_G_TRANSITION_TRACK

#include "EMCommand.h"

class EMGTrack;
class TrackInfo;
class TrackView;

class CommandAddGTransitionTrack : public EMCommand
{
public:
	CommandAddGTransitionTrack(TrackView* p_opTrackView, TrackInfo* p_opTrackInfo);
	~CommandAddGTransitionTrack();
	EMCommand* CloneAndClear();
	void ExecuteE() {};
	void* ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree);
	const char* GetShortDescription();
	bool IsUndoable();
	bool RequiresParameters();
	void SetTrack(EMGTrack* p_opTrack);
	void UndoE();

private:
	EMGTrack* m_opTrack;
	TrackInfo* m_opTrackInfo;
	TrackView* m_opTrackView;
};

#endif

