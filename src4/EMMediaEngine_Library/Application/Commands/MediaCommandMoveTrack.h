/*******************************************************
* Creator: Jesper Svensson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_MOVE_TRACK
#define __MEDIA_COMMAND_MOVE_TRACK

#include "EMCommand.h"

class EMMediaTrack;

class MediaCommandMoveTrack : public EMCommand
{
public:
	MediaCommandMoveTrack();
	//Parameter 1 = track ID, parameter 2 = new track name
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
	void ExecuteE() {};
	bool RequiresParameters();
	void UndoE();
	bool IsUndoable();

private:
	EMMediaTrack* m_opRecentTrack;	
	uint32 m_oRecentPrio;
};

#endif
