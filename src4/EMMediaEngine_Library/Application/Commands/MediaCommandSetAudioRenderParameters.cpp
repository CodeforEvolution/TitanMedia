#include "MediaCommandSetAudioRenderParameters.h"

#include "EMWinAudioDiskWriter.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"


MediaCommandSetAudioRenderParameters::MediaCommandSetAudioRenderParameters()
{
}

void* MediaCommandSetAudioRenderParameters::ExecuteE(void* p_vSampleRate, void* p_vNumBits, void* p_vIsMono)
{
	int32 vSampleRate = *static_cast<int32*>(p_vSampleRate);
	char vNumBits = *static_cast<char*>(p_vNumBits);
	bool vIsMono = *static_cast<bool*>(p_vIsMono);

	if(EMWinAudioDiskWriter::Instance() -> IsRunning())
	{
		m_vRet = false;
	}
	else
	{
		m_vRet = EMWinAudioDiskWriter::Instance() -> InitConverter(vSampleRate, vNumBits, vIsMono);
	}
	
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);

	return &m_vRet;
}

bool MediaCommandSetAudioRenderParameters::RequiresParameters()
{
	return true;
}

bool MediaCommandSetAudioRenderParameters::IsUndoable()
{
	return false;
}