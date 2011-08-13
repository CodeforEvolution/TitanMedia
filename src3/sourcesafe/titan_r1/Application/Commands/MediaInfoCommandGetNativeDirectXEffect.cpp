#include "MediaInfoCommandGetNativeDirectXEffect.h"

#include "EMMediaEffectTrackRepository.h"
#include "EMPlugin.h"
#include "EMMediaEffectTrack.h"

#include <streams.h>

MediaInfoCommandGetNativeDirectXEffect::MediaInfoCommandGetNativeDirectXEffect()
{
}

MediaInfoCommandGetNativeDirectXEffect::~MediaInfoCommandGetNativeDirectXEffect()
{
}

void* MediaInfoCommandGetNativeDirectXEffect::ExecuteE(void* p_upMediaEffectID, void*, void*)
{
	int32 vMediaEffectID = *static_cast<int32*>(p_upMediaEffectID);

	EMMediaEffectTrackRepository* opFXTrackRep = EMMediaEffectTrackRepository::Instance();

	EMPlugin* opWantedPlugin = NULL;

	opFXTrackRep -> Rewind();
	while(opFXTrackRep -> Current() != NULL)
	{
		EMMediaEffectTrack* opFXTrack = opFXTrackRep -> Current();
		opWantedPlugin = opFXTrack -> FindEffect(vMediaEffectID);
		if(opWantedPlugin != NULL && !opFXTrack -> IsObjectDeleted())
			break;
		opFXTrackRep -> Next();
	}

	if(opWantedPlugin != NULL)
	{
		m_opReturnValue = static_cast<IBaseFilter*>(opWantedPlugin -> GetNativeEffect());
	}
	else
		EMDebugger("ERROR! Could not find the effect with the specified ID!");

	return static_cast<void*>(&m_opReturnValue);
}

bool MediaInfoCommandGetNativeDirectXEffect::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetNativeDirectXEffect::IsUndoable()
{
	return false;
}