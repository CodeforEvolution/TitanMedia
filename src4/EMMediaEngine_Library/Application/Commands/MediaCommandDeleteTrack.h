/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_DELETE_TRACK
#define __MEDIA_COMMAND_DELETE_TRACK

#include "EMCommand.h"

class MediaCommandDeleteTrack : public EMCommand
{
public:
	MediaCommandDeleteTrack();
	MediaCommandDeleteTrack(int32 p_vMediaTrackID, int32 p_vEffectTrackID);

	void ExecuteE() {};
	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree);
	bool RequiresParameters();
	bool IsUndoable();
	void UndoE();
	EMCommand* CloneAndClear();

private:
	int32 m_vDeletedMediaTrackID;
	int32 m_vFxID;
};

#endif
