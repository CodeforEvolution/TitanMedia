/*******************************************************
* Creator: Jesper Svensson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SET_MAGIC_SPEED_VALUE
#define __MEDIA_COMMAND_SET_MAGIC_SPEED_VALUE

#include "EMCommand.h"

class EMMediaClip;
class EMMediaTrack;

class  MediaCommandSetMagicSpeedValue : public EMCommand
{
public:
	
	//Used for command instantiation
	MediaCommandSetMagicSpeedValue();

	void ExecuteE() {};

	//parameter 1 = clip ID <int32>, parameter 2 = new song time in frames <int64>, parameter 3 = new track id (if changed) <int32>
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
	bool RequiresParameters();
	bool IsUndoable();
//	void UndoE();
//	EMCommand* CloneAndClear();

private:
};

#endif
