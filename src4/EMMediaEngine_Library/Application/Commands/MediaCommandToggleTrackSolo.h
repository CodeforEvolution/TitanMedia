/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_TOGGLE_TRACK_SOLO
#define __MEDIA_COMMAND_TOGGLE_TRACK_SOLO

#include "EMCommand.h"

class EMMediaTrack;

class MediaCommandToggleTrackSolo : public EMCommand
{
public:
	MediaCommandToggleTrackSolo(EMMediaTrack* p_opRecentTrack = NULL);
	void ExecuteE() {};
	
	//Parameter 1 = track ID, parameter 2 = new track pan value
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
	bool RequiresParameters();
	void UndoE();
	bool IsUndoable();
	EMCommand* CloneAndClear();

private:
	EMMediaTrack* m_opRecentTrack;	
};

#endif
