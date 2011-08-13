#include "MediaCommandShowEffectDialog.h"

#include "EMMediaEffectTrack.h"
#include "EMMediaEffectTrackRepository.h"
#include "EMPlugin.h"
#include "EMWinDirectXPlugin.h"

MediaCommandShowEffectDialog::MediaCommandShowEffectDialog()
{
}

//Takes two IDs - the plugin entry id and the effect track id
void* MediaCommandShowEffectDialog::ExecuteE(void* p_upPluginID, void* p_upEffectTrackID, void*)
{
	int32 vPluginID = *static_cast<int32*>(p_upPluginID); //Existing Effect ID
	int32 vFXTrackID = *static_cast<int32*>(p_upEffectTrackID); //Existing track ID

	EMMediaEffectTrack* opFXTrack = EMMediaEffectTrackRepository::Instance() -> Find(vFXTrackID);
	if(opFXTrack == NULL)
		EMDebugger("ERROR! Could not locate the specified effect track ID!");
 
	EMPlugin* opPlugin = opFXTrack -> FindEffect(vPluginID);
	if(opPlugin == NULL)
		EMDebugger("ERROR! Could not locate the specified plugin ID!");

	if(!opPlugin -> ShowDialog())
	{
//		EMDebugger("ERROR! Unknown plugin, or ShowDialog not implemented for that type of plugin!");
	}

	return NULL; 
}	

bool MediaCommandShowEffectDialog::RequiresParameters()
{
	return true;
}

bool MediaCommandShowEffectDialog::IsUndoable()
{
	return false;
}
