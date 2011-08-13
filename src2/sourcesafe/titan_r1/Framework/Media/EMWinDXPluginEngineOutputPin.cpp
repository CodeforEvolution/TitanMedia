#include "EMWinDXPluginEngineOutputPin.h"

#include "EMGlobals.h"
#include "EMMediaGlobals.h"
#include "EMMediaDataBuffer.h"
#include "EMSettingsRepository.h"
#include "EMSettingIDs.h"
#include "EMMediaEngine.h"

#include <streams.h>

EMWinDXPluginEngineOutputPin* EMWinDXPluginEngineOutputPin::Create(EMWinDXPluginEngineFilter* p_opParentFilter)
{
	HRESULT vResult = S_OK;
	return EM_new EMWinDXPluginEngineOutputPin(&vResult, p_opParentFilter, L"Audio Out");
}


EMWinDXPluginEngineOutputPin::EMWinDXPluginEngineOutputPin(HRESULT *phr, EMWinDXPluginEngineFilter* p_opParentFilter, LPCWSTR pName)
	:	CBaseOutputPin(NAME(""), p_opParentFilter, &m_cSharedState, phr, pName),
		m_opParentFilter(p_opParentFilter),
		m_oFilterName("Untitled"),
		m_vNumberOfOutChannels(0)
{
	m_bTryMyTypesFirst = true;
}

EMWinDXPluginEngineOutputPin::~EMWinDXPluginEngineOutputPin()
{
}

void EMWinDXPluginEngineOutputPin::SetName(string p_oName)
{
	m_oFilterName = p_oName;
}

HRESULT EMWinDXPluginEngineOutputPin::Active()
{
	CAutoLock lock(m_opParentFilter -> GetLock());
	//CAutoLock lShared(&m_cSharedState);

	if(m_opParentFilter -> IsActive())
		return S_FALSE;

	HRESULT hr = CBaseOutputPin::Active();
	
	if(hr != S_OK)
		return hr;
	return NOERROR;
}

HRESULT EMWinDXPluginEngineOutputPin::BreakConnect()
{
	CAutoLock lock(m_opParentFilter -> GetLock());
	HRESULT vResult = CBaseOutputPin::BreakConnect();
	if(FAILED(vResult))
		return vResult;
	return NOERROR;
}

HRESULT EMWinDXPluginEngineOutputPin::CompleteConnect(IPin *pReceivePin)
{
	CAutoLock lock(m_opParentFilter -> GetLock());
	//CAutoLock lShared(&m_cSharedState);
	HRESULT h = CBaseOutputPin::CompleteConnect(pReceivePin);
	if(FAILED(h))
		return h;

	AM_MEDIA_TYPE uMediaType;
	HRESULT vResult = pReceivePin -> ConnectionMediaType(&uMediaType);
	if(uMediaType.formattype == FORMAT_WaveFormatEx)
		m_vNumberOfOutChannels = ((WAVEFORMATEX*) uMediaType.pbFormat) -> nChannels;
	else
		return E_FAIL;

	return h;
}

HRESULT EMWinDXPluginEngineOutputPin::DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *pProperties)
{
	CritCheckIn(m_opParentFilter -> GetLock());

	pProperties -> cBuffers = 2;
	pProperties -> cbBuffer = *static_cast<int32*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_BUFFER_SIZE));

    ALLOCATOR_PROPERTIES Actual;
    HRESULT hr = pAlloc -> SetProperties(pProperties, &Actual);
    if(FAILED(hr)) 
	{
        return hr;
	}

	if(Actual.cbBuffer < pProperties -> cbBuffer) 
	{
        return E_FAIL;
	}
	return NOERROR; //pAlloc -> Commit();
}

HRESULT EMWinDXPluginEngineOutputPin::GetMediaType(int p_vPosition, CMediaType *p_opMediaTypeObject)
{
	CAutoLock lock(m_opParentFilter -> GetLock());

	if(p_vPosition == 0)
	{
		//First, allocate a new WAVEFORMATEX structure and get a pointer to it...
		WAVEFORMATEX* sWaveFormatStructure = reinterpret_cast<WAVEFORMATEX*>(p_opMediaTypeObject -> AllocFormatBuffer(sizeof(WAVEFORMATEX)));
		//Then fill it out with the PCM format information
		sWaveFormatStructure -> wFormatTag = WAVE_FORMAT_PCM;
		sWaveFormatStructure -> nChannels = EM_AUDIO_NUM_CHANNELS;
		sWaveFormatStructure -> nSamplesPerSec = (uint32) 44100; //*(static_cast<int32*>(m_opFilter -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_FRAMERATE)));
		sWaveFormatStructure -> wBitsPerSample = EM_AUDIO_SAMPLESIZE * 8;        
		sWaveFormatStructure -> nBlockAlign = static_cast<WORD>((sWaveFormatStructure -> wBitsPerSample * sWaveFormatStructure -> nChannels) / 8);
		sWaveFormatStructure -> nAvgBytesPerSec = sWaveFormatStructure -> nBlockAlign * sWaveFormatStructure -> nSamplesPerSec;
		sWaveFormatStructure -> cbSize = 0;
		//And then fill out the CMediaType object with information based on that WAVEFORMATEX structure
		if(FAILED(CreateAudioMediaType(sWaveFormatStructure, p_opMediaTypeObject, FALSE)))
			return E_UNEXPECTED;
	}
	else if(p_vPosition == 1)
	{
		//First, allocate a new WAVEFORMATEX structure and get a pointer to it...
		WAVEFORMATEX* sWaveFormatStructure = reinterpret_cast<WAVEFORMATEX*>(p_opMediaTypeObject -> AllocFormatBuffer(sizeof(WAVEFORMATEX)));
		//Then fill it out with the PCM format information
		sWaveFormatStructure -> wFormatTag = WAVE_FORMAT_PCM;
		sWaveFormatStructure -> nChannels = 1;
		sWaveFormatStructure -> nSamplesPerSec = (uint32) 44100; //*(static_cast<int32*>(m_opFilter -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_FRAMERATE)));
		sWaveFormatStructure -> wBitsPerSample = EM_AUDIO_SAMPLESIZE * 8;        
		sWaveFormatStructure -> nBlockAlign = static_cast<WORD>((sWaveFormatStructure -> wBitsPerSample * sWaveFormatStructure -> nChannels) / 8);
		sWaveFormatStructure -> nAvgBytesPerSec = sWaveFormatStructure -> nBlockAlign * sWaveFormatStructure -> nSamplesPerSec;
		sWaveFormatStructure -> cbSize = 0;
		//And then fill out the CMediaType object with information based on that WAVEFORMATEX structure
		if(FAILED(CreateAudioMediaType(sWaveFormatStructure, p_opMediaTypeObject, FALSE)))
			return E_UNEXPECTED;
	}
 	else
		return VFW_S_NO_MORE_ITEMS;
	return S_OK;
}

HRESULT EMWinDXPluginEngineOutputPin::CheckMediaType(const CMediaType *pMediaType)
{
	CAutoLock lock(m_opParentFilter -> GetLock());
	return S_OK;
}

STDMETHODIMP EMWinDXPluginEngineOutputPin::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
	HRESULT h = CBaseOutputPin::NonDelegatingQueryInterface(riid, ppv);
	return h;
}

bool EMWinDXPluginEngineOutputPin::ProcessBuffer(EMMediaDataBuffer* p_opBuffer)
{
	IMediaSample *pSample = NULL;
	HRESULT hr = GetDeliveryBuffer(&pSample, NULL, NULL, 0);
	if(FAILED(hr))
		return false;
	else
	{
		BYTE* data;
		pSample -> GetPointer(&data);
		memcpy(data, p_opBuffer -> Data(), p_opBuffer -> m_vSizeUsed);
		hr = Deliver(pSample);
		pSample -> Release();
	}
	return true;
}
