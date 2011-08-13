#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMWinAudioInputPin.h"
#include "EMMediaDataBuffer.h"
//#include "EMWinMediaOutputFilterInterface.h"
#include "EMWinMediaInputFilter.h"
#include "EMMediaTimer.h"
#include "EMMediaUtility.h"
#include "EMSettingsRepository.h"
#include "EMSettingIDs.h"
#include "EMMediaUtility.h"
#include "EMDirectXFilter.h"

EMWinAudioInputPin::EMWinAudioInputPin(HRESULT *phr, EMDirectXFilter* p_opParentFilter, string p_oName)
	:	EMWinMediaInputPin(phr, p_opParentFilter, L"Audio input pin", &m_oLock),
		m_opParentFilter(p_opParentFilter)
{
}

EMWinAudioInputPin::~EMWinAudioInputPin()
{
	m_opParentFilter = NULL;
}

STDMETHODIMP EMWinAudioInputPin::GetAllocatorRequirements(ALLOCATOR_PROPERTIES *pProps)
{

	return CRenderedInputPin::GetAllocatorRequirements(pProps);
/*	pProps -> cBuffers = 2;
	pProps -> cbBuffer = 4096;
	pProps -> cbAlign = 4;
	pProps -> cbPrefix = 0;

	return S_OK;*/
}



bool EMWinAudioInputPin::OnRun(uint64 p_vStartingSongTime)
{
	return true;
}

bool EMWinAudioInputPin::OnEndOfStream()
{
	return true;
}

bool EMWinAudioInputPin::ReportFormat(CMediaType* p_opFormat)
{
	//First, allocate a new WAVEFORMATEX structure and get a pointer to it...
	WAVEFORMATEX* sWaveFormatStructure = reinterpret_cast<WAVEFORMATEX*>(p_opFormat -> AllocFormatBuffer(sizeof(WAVEFORMATEX)));

	//Then fill it out with the PCM format information
	sWaveFormatStructure -> wFormatTag = WAVE_FORMAT_PCM;
    sWaveFormatStructure -> nChannels = EM_AUDIO_NUM_CHANNELS;
    sWaveFormatStructure -> nSamplesPerSec = 44100; //(uint32) *(static_cast<int32*>(m_opFilter -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_FRAMERATE)));
	sWaveFormatStructure -> wBitsPerSample = EM_AUDIO_SAMPLESIZE * 8;        
	sWaveFormatStructure -> nBlockAlign = static_cast<WORD>((sWaveFormatStructure -> wBitsPerSample * sWaveFormatStructure -> nChannels) / 8);
    sWaveFormatStructure -> nAvgBytesPerSec = sWaveFormatStructure -> nBlockAlign * sWaveFormatStructure -> nSamplesPerSec;
    sWaveFormatStructure -> cbSize = 0;

	//And then fill out the CMediaType object with information based on that WAVEFORMATEX structure
	if(FAILED(CreateAudioMediaType(sWaveFormatStructure, p_opFormat, FALSE)))
		return false;
	return true;
}

bool EMWinAudioInputPin::BufferReceived(IMediaSample* p_opMediaSample)
{
	return m_opParentFilter -> BufferReceived(p_opMediaSample);
}

#endif