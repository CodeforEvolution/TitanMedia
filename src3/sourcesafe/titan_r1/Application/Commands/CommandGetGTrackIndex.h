/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_GET_G_TRACK_INDEX
#define __COMMAND_GET_G_TRACK_INDEX

#include "EMCommand.h"

class CommandGetGTrackIndex : public EMCommand
{
public:
	CommandGetGTrackIndex();
	void ExecuteE() {};
	void* ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree);
	bool IsUndoable();
	bool RequiresParameters();

private:
	uint32 m_vTrackIndex;
};

#endif
