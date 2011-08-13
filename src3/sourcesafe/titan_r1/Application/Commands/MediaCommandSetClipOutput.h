/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SET_CLIP_OUTPUT
#define __MEDIA_COMMAND_SET_CLIP_OUTPUT

#include "EMCommand.h"

class MediaCommandSetClipOutput : public EMCommand
{
public:
	MediaCommandSetClipOutput(int32 p_vRecentClipID = -1, int32 p_vRecentOutputID = -1);
	void ExecuteE() {};
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
	bool RequiresParameters();
	void UndoE();
	bool IsUndoable();
	EMCommand* CloneAndClear();
	
private:
	int32 m_vRecentClipID;
	int32 m_vRecentOutputID;
};

#endif
