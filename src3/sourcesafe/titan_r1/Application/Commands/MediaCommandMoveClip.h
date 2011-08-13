/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_MOVE_CLIP
#define __MEDIA_COMMAND_MOVE_CLIP

#include "EMCommand.h"

class EMMediaClip;
class EMMediaTrack;

class MediaCommandMoveClip : public EMCommand
{
public:
	MediaCommandMoveClip(EMMediaClip* p_opClip = NULL, int64 p_vOldStart = 0, EMMediaTrack* p_opRecentTrack = NULL);
	void ExecuteE() {};

	//parameter 1 = clip ID <int32>, parameter 2 = new song time in frames <int64>, parameter 3 = new track id (if changed) <int32>
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
	bool RequiresParameters();
	void UndoE();
	bool IsUndoable();
	EMCommand* CloneAndClear();

private:
	EMMediaClip* m_opClip;
	int64 m_vOldStart;
	EMMediaTrack* m_opRecentTrack;
};

#endif
