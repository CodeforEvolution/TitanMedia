#include "MediaInfoCommandGetEncoderProperties.h"

#include "EMMediaEngine.h"
#include "EMWinMediaSingletonMuxer.h"
#include "EMWinQuickTimeWriterSuper.h"

MediaInfoCommandGetEncoderProperties::MediaInfoCommandGetEncoderProperties()
{
}

void* MediaInfoCommandGetEncoderProperties::ExecuteE(void* p_vEncoderID, void*, void*)
{
	int32 vpEncoderID = *static_cast<int32*>(p_vEncoderID);
	if(EMWinMediaSingletonMuxer::Instance() ->m_vCurrentlyUsedFamily == EM_FAMILY_AVI)
		m_vEncoderPropertyPointer = EMWinMediaSingletonMuxer::Instance() -> GetCompressionCapabilities(vpEncoderID);
	else if(EMWinMediaSingletonMuxer::Instance() ->m_vCurrentlyUsedFamily == EM_FAMILY_QUICKTIME)
		m_vEncoderPropertyPointer = EMWinQuickTimeWriterSuper::Instance() -> GetCompressionCapabilities(vpEncoderID);

	return static_cast<void*>(m_vEncoderPropertyPointer);
}

bool MediaInfoCommandGetEncoderProperties::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetEncoderProperties::IsUndoable()
{
	return false;
}