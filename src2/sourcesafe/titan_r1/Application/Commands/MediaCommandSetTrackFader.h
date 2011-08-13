/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SET_TRACK_FADER
#define __MEDIA_COMMAND_SET_TRACK_FADER

#include "EMCommand.h"

class EMMediaTrack;

class MediaCommandSetTrackFader : public EMCommand
{
public:
	MediaCommandSetTrackFader(EMMediaTrack* p_opRecentTrack = NULL, int64 p_vRecentFader = 127);
	void ExecuteE() {};
	
	//Parameter 1 = track ID, parameter 2 = new track fader value
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
	bool RequiresParameters();
	void UndoE();
	bool IsUndoable();
	EMCommand* CloneAndClear();

private:
	EMMediaTrack* m_opRecentTrack;	
	int32 m_vRecentFader;
};

#endif
