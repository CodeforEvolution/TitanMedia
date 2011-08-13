/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SET_TRACK_VELOCITY_OFFSET
#define __MEDIA_COMMAND_SET_TRACK_VELOCITY_OFFSET

#include "EMCommand.h"

class EMMediaMIDITrack;

class MediaCommandSetTrackVelocityOffset : public EMCommand
{
public:
	MediaCommandSetTrackVelocityOffset(EMMediaMIDITrack* p_opRecentMIDITrack = NULL, int32 p_vRecentOffset = 0);
	void ExecuteE() {};
	
	//Parameter 1 = track ID, parameter 2 = new track pan value
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
	bool RequiresParameters();
	void UndoE();
	bool IsUndoable();
	EMCommand* CloneAndClear();

private:
	EMMediaMIDITrack* m_opRecentMIDITrack;	
	int32 m_vRecentOffset;
};

#endif
