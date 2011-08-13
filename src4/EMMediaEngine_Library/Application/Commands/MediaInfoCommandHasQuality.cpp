#include "MediaInfoCommandHasQuality.h"

#include "EMMediaEngine.h"
#include "EMWinMediaSingletonMuxer.h"

#include <list>

MediaInfoCommandHasQuality::MediaInfoCommandHasQuality()
{
}

void* MediaInfoCommandHasQuality::ExecuteE(void* p_vEncoderID, void*, void*)
{
	int32 vpEncoderID = *static_cast<int32*>(p_vEncoderID);

	if(EMWinMediaSingletonMuxer::Instance() ->m_vCurrentlyUsedFamily == EM_FAMILY_QUICKTIME)
	{
		//Temporarily set to false
		vHasQuality = true;
		return &vHasQuality;
	}

	vHasQuality = EMWinMediaSingletonMuxer::Instance() -> SetCompressionQuality(0.75, vpEncoderID);
	return &vHasQuality;
}

bool MediaInfoCommandHasQuality::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandHasQuality::IsUndoable()
{
	return false;
}