#include "MediaCommandCreateEffectTrack.h"

#include "EMMediaEffectTrack.h"
#include "EMMediaEffectTrackRepository.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"

MediaCommandCreateEffectTrack::MediaCommandCreateEffectTrack()
	:	m_vReturnValue(-1)
{
}

//Takes a string (The name of the new effect track) and returns an ID number of the newly created effect track)
void* MediaCommandCreateEffectTrack::ExecuteE(void* p_upName, void*, void*)
{
	string oTrackName(p_upName != NULL ? static_cast<const char*>(p_upName) : "Untitled effect track");

	EMMediaEffectTrack* opFXTrack = EM_new EMMediaEffectTrack(EM_TYPE_ANY);
	if(opFXTrack == NULL)
	{
		EMDebugger(string(string("ERROR! Could not create effect track \"") + oTrackName + string("\"")).c_str());
		return NULL;
	}
	opFXTrack -> SetName(oTrackName);

	if(! opFXTrack -> InitCheckE())
		EMDebugger("ERROR! Failed to initialize effect track!");

	EMMediaEffectTrackRepository::Instance() -> Add(opFXTrack);
	m_vReturnValue = opFXTrack -> GetID();
	opFXTrack = NULL;

	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);

	return static_cast<void*>(&m_vReturnValue);
}

bool MediaCommandCreateEffectTrack::RequiresParameters()
{
	return true;
}

void MediaCommandCreateEffectTrack::UndoE()
{
}

bool MediaCommandCreateEffectTrack::IsUndoable()
{
	return false;
}

EMCommand* MediaCommandCreateEffectTrack::CloneAndClear()
{
	return new MediaCommandCreateEffectTrack();
}

