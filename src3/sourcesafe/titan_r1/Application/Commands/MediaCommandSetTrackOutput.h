/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_SET_TRACK_OUTPUT
#define __MEDIA_COMMAND_SET_TRACK_OUTPUT

#include "EMCommand.h"

class EMMediaTrack;
class EMRealtimeOutputDescriptor;
class EMMediaEffectTrack;

class MediaCommandSetTrackOutput : public EMCommand
{
public:
	MediaCommandSetTrackOutput(int32 p_vRecentTrackID = -1, int32 p_vRecentOutputID = -1);
	void ExecuteE() {};
	void* ExecuteE(void* p_upTrackID, void* p_upOutputID, void*);
	bool RequiresParameters();
	void UndoE();
	bool IsUndoable();
	EMCommand* CloneAndClear();

	bool SetTrackPhysicalOutput(EMMediaTrack* p_opTrack, EMRealtimeOutputDescriptor* p_opPhysicalOutput);
	bool SetTrackEffectOutput(EMMediaTrack* p_opTrack, EMMediaEffectTrack* p_opEffectTrack);
	
private:
	int32 m_vRecentTrackID;
	int32 m_vRecentOutputID;
};

#endif
