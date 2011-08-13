#include "MediaCommandSetEffectDryWetMix.h"

#include "EMMediaEffectTrack.h"
#include "EMMediaEffectTrackRepository.h"
#include "EMPlugin.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"

MediaCommandSetEffectDryWetMix::MediaCommandSetEffectDryWetMix(int32 p_vPlugin, int32 p_vTrack, uint32 p_vMix)
	:	m_vPluginID(p_vPlugin),
		m_vTrackID(p_vTrack),
		m_vOldDryWetMix(p_vMix)
{
}

//Takes two IDs - the plugin entry id and the effect track id, and returns the id of the new instantiated plugin
void* MediaCommandSetEffectDryWetMix::ExecuteE(void* p_upPluginID, void* p_upEffectTrackID, void* p_upDryWetRatio)
{
	int32 vPluginID = *static_cast<int32*>(p_upPluginID); //Existing Effect ID
	int32 vFXTrackID = *static_cast<int32*>(p_upEffectTrackID); //Existing track ID
	uint32 vDryWetMix = *static_cast<uint32*>(p_upDryWetRatio); //The dry/wet ratio. 0 = dry, 100 = wet.

	EMMediaEffectTrack* opFXTrack = EMMediaEffectTrackRepository::Instance() -> Find(vFXTrackID);
	if(opFXTrack == NULL)
		EMDebugger("ERROR! Could not locate the specified effect track ID!");

	EMPlugin* opPlugin = opFXTrack -> FindEffect(vPluginID);
	if(opPlugin == NULL)
		EMDebugger("ERROR! Could not locate the specified plugin ID!");

	if(vDryWetMix > 100)
		vDryWetMix = 100;

	m_vOldDryWetMix = (uint32) (opPlugin -> GetDryWetMix() * 100.0);
	m_vPluginID = opPlugin -> GetID();
	m_vTrackID = opFXTrack -> GetID();
	opPlugin -> SetDryWetMix(((float) vDryWetMix) / 100.0);
	
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);

	return NULL;
}	

bool MediaCommandSetEffectDryWetMix::RequiresParameters()
{
	return true;
}

void MediaCommandSetEffectDryWetMix::UndoE()
{
	ExecuteE(&m_vPluginID, &m_vTrackID, &m_vOldDryWetMix);
}

bool MediaCommandSetEffectDryWetMix::IsUndoable()
{
	return false;
}

EMCommand* MediaCommandSetEffectDryWetMix::CloneAndClear()
{
	return EM_new MediaCommandSetEffectDryWetMix(m_vPluginID, m_vTrackID, m_vOldDryWetMix);
}

