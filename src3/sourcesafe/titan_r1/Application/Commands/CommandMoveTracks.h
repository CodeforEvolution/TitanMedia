/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_MOVE_TRACKS
#define __COMMAND_MOVE_TRACKS

#include "EMCommand.h"

#include <vector>

class EMGTrack;
class TrackWindow;

class CommandMoveTracks : public EMCommand
{
public:
	CommandMoveTracks(TrackWindow* p_opTrackWindow);
	CommandMoveTracks(TrackWindow* p_opTrackWindow, vector<EMGTrack*> p_oOriginalTrackOrder);
	EMCommand* CloneAndClear();
	void ExecuteE() {};
	void* ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree);
	const char* GetShortDescription();
	bool IsUndoable();
	bool RequiresParameters();
	void UndoE();

private:
	TrackWindow* m_opTrackWindow;
	vector<EMGTrack*> m_oOriginalTrackOrder;
};

#endif
