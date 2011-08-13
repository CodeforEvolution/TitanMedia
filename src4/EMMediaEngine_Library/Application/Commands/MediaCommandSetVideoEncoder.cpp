#include "MediaCommandSetVideoEncoder.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMWinMediaSingletonMuxer.h"
#include "EMWinQuickTimeWriterSuper.h"
MediaCommandSetVideoEncoder::MediaCommandSetVideoEncoder()
{
}

void* MediaCommandSetVideoEncoder::ExecuteE(void* p_vEncoderID, void*, void*)
{
	int32 vpEncoderID = *static_cast<int32*>(p_vEncoderID);

	if(EMWinMediaSingletonMuxer::Instance() ->m_vCurrentlyUsedFamily == EM_FAMILY_QUICKTIME)
	{
		EMWinQuickTimeWriterSuper::Instance() -> SetVideoEncoderToUse(vpEncoderID);
		return NULL;
	}

	EMWinMediaSingletonMuxer::Instance() -> m_vVideoEncoderID = vpEncoderID;
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);

	//EMWinMediaSingletonMuxer::Instance() ->GetEncoderPropertyPages(vpEncoderID, true);
	return NULL;
}

bool MediaCommandSetVideoEncoder::RequiresParameters()
{
	return true;
}

bool MediaCommandSetVideoEncoder::IsUndoable()
{
	return false;
}