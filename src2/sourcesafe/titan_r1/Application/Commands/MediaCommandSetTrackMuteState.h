/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SET_TRACK_MUTE_STATE
#define __MEDIA_COMMAND_SET_TRACK_MUTE_STATE

#include "EMCommand.h"

class EMMediaTrack;

class MediaCommandSetTrackMuteState : public EMCommand
{
public:
	MediaCommandSetTrackMuteState(EMMediaTrack* p_opRecentTrack = NULL, EMMuteState p_eOldMuteState = EM_MUTE_OFF);
	void ExecuteE() {};
	
	//Parameter 1 = track ID, parameter 2 = new track pan value
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
	bool RequiresParameters();
	void UndoE();
	bool IsUndoable();
	EMCommand* CloneAndClear();

private:
	EMMediaTrack* m_opRecentTrack;	
	EMMuteState m_eOldMuteState;
};

#endif
