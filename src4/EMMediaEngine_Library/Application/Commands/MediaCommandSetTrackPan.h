/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SET_TRACK_PAN
#define __MEDIA_COMMAND_SET_TRACK_PAN

#include "EMCommand.h"

class EMMediaTrack;

class MediaCommandSetTrackPan : public EMCommand
{
public:
	MediaCommandSetTrackPan(EMMediaTrack* p_opRecentTrack = NULL, int64 p_vRecentPan = 0);
	void ExecuteE() {};
	
	//Parameter 1 = track ID, parameter 2 = new track pan value
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
	bool RequiresParameters();
	void UndoE();
	bool IsUndoable();
	EMCommand* CloneAndClear();

private:
	EMMediaTrack* m_opRecentTrack;	
	int32 m_vRecentPan;
};

#endif
