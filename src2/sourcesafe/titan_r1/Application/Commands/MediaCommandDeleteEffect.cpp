#include "MediaCommandDeleteEffect.h"

#include "EMMediaEffectTrack.h"
#include "EMMediaEffectTrackRepository.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"

MediaCommandDeleteEffect::MediaCommandDeleteEffect()
{
}

//Takes two IDs - the plugin entry id and the effect track id, and returns the id of the new instantiated plugin
void* MediaCommandDeleteEffect::ExecuteE(void* p_upEffectTrackID, void* p_upSlotNumber, void*)
{
	int32 vFXTrackID = *static_cast<int32*>(p_upEffectTrackID); //Existing track ID
	int32 vSlotNumber = *static_cast<uint32*>(p_upSlotNumber); //Already occupied slot number

	EMMediaEffectTrack* opFXTrack = EMMediaEffectTrackRepository::Instance() -> Find(vFXTrackID);

	if(opFXTrack == NULL)
		EMDebugger("ERROR! Could not locate the specified effect track ID!");

	if(! opFXTrack -> DeleteEffect(vSlotNumber))
		EMDebugger("ERROR! Failed to delete effect. Are you sure the slot was occupied?");
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
	return NULL;
}	

bool MediaCommandDeleteEffect::RequiresParameters()
{
	return true;
}

bool MediaCommandDeleteEffect::IsUndoable()
{
	return false;
}
