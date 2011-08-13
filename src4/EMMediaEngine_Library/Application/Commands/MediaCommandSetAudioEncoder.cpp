#include "MediaCommandSetAudioEncoder.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMWinMediaSingletonMuxer.h"
#include "EMWinQuickTimeWriterSuper.h"
MediaCommandSetAudioEncoder::MediaCommandSetAudioEncoder()
{
}

void* MediaCommandSetAudioEncoder::ExecuteE(void* p_vEncoderID, void*, void*)
{
	int32 vpEncoderID = *static_cast<int32*>(p_vEncoderID);

	if(EMWinMediaSingletonMuxer::Instance() ->m_vCurrentlyUsedFamily == EM_FAMILY_QUICKTIME)
	{
		EMWinQuickTimeWriterSuper::Instance() -> SetAudioEncoderToUse(vpEncoderID);
		return NULL;
	}

	EMWinMediaSingletonMuxer::Instance() -> m_vAudioEncoderID = vpEncoderID;

	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);

	return NULL;
}

bool MediaCommandSetAudioEncoder::RequiresParameters()
{
	return true;
}

bool MediaCommandSetAudioEncoder::IsUndoable()
{
	return false;
}