/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_CLONE_CLIP
#define __MEDIA_COMMAND_CLONE_CLIP

#include "EMCommand.h"

class EMMediaClip;
class EMMediaTrack;

class MediaCommandCloneClip : public EMCommand
{
public:
	
	//Used for command instantiation
	MediaCommandCloneClip();

	//Used for Undo
	MediaCommandCloneClip(int32 p_vClipCopyID);
	void ExecuteE() {};

	//parameter 1 = clip ID <int32>, parameter 2 = new song time in frames <int64>, parameter 3 = new track id (if changed) <int32>
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
	bool RequiresParameters();
	bool IsUndoable();
	void UndoE();
	EMCommand* CloneAndClear();

private:
	int32 m_vReturnValue;

	int32 m_vClipCopyID;
};

#endif
