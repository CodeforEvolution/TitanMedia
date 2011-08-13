#include "MediaCommandSetEncoderProperty.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMWinMediaSingletonMuxer.h"
#include "EMWinQuickTimeWriterSuper.h"

MediaCommandSetEncoderProperty::MediaCommandSetEncoderProperty()
{
}

void* MediaCommandSetEncoderProperty::ExecuteE(void* p_vEncoderID, void* p_vPropertyID, void*)
{
	int32 vpEncoderID = *static_cast<int32*>(p_vEncoderID);
	int32 vpPropertyID = *static_cast<int32*>(p_vPropertyID); //Incoming value is one greater than should be
	
	//Because a 0 would create chaos we choose to decimate instead
	vpPropertyID --;

	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);

	if(EMWinMediaSingletonMuxer::Instance() ->m_vCurrentlyUsedFamily == EM_FAMILY_QUICKTIME)
	{
		EMWinQuickTimeWriterSuper::Instance() -> SetCompressionCapabilityInfo(vpEncoderID, vpPropertyID);
		return NULL;
	}

	EMWinMediaSingletonMuxer::Instance() -> SetCompressionCapabilityInfo(vpEncoderID, vpPropertyID);


	return NULL;
}

bool MediaCommandSetEncoderProperty::RequiresParameters()
{
	return true;
}

bool MediaCommandSetEncoderProperty::IsUndoable()
{
	return false;
}
