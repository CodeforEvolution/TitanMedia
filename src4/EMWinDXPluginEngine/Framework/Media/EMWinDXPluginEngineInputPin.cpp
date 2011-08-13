	#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMWinDXPluginEngineInputPin.h"
#include "EMMediaDataBuffer.h"
#include "EMWinDXPluginEngineFilter.h"

//#include <uuids.h>

EMWinDXPluginEngineInputPin* EMWinDXPluginEngineInputPin::Create(EMWinDXPluginEngineFilter* p_opParentFilter)
{
	HRESULT vResult = S_OK;
	return EM_new EMWinDXPluginEngineInputPin(&vResult, p_opParentFilter, L"Audio In");
}

EMWinDXPluginEngineInputPin::EMWinDXPluginEngineInputPin(HRESULT *phr, EMWinDXPluginEngineFilter* p_opParentFilter, LPCWSTR pName)
	:	CRenderedInputPin(NAME("Debug name"), p_opParentFilter, &m_oLock, phr, pName),
		m_opFilter(p_opParentFilter),
		m_opConnectedMediaType(NULL),
		m_vMediaTimeStart(0),
		m_opStoredBuffer(NULL),
		m_vBufferWaiting(false),
		m_oFilterName("Untitled"),
		m_vNumberOfInChannels(0)
{
	m_bTryMyTypesFirst = true;
}

EMWinDXPluginEngineInputPin::~EMWinDXPluginEngineInputPin()
{
}

void EMWinDXPluginEngineInputPin::SetName(string p_oName)
{
	m_oFilterName = p_oName;
}

HRESULT EMWinDXPluginEngineInputPin::CheckMediaType(const CMediaType* p_opMediaTypeObject)
{
	CAutoLock lock(m_opFilter -> GetLock());
	AM_MEDIA_TYPE* upMediaType = (AM_MEDIA_TYPE*) p_opMediaTypeObject;
	if(upMediaType -> formattype == FORMAT_WaveFormatEx)
		m_vNumberOfInChannels = ((WAVEFORMATEX*) upMediaType -> pbFormat) -> nChannels;
	else
		return S_FALSE;
	return S_OK;
}

HRESULT EMWinDXPluginEngineInputPin::GetMediaType(int p_vPosition, CMediaType *p_opMediaTypeObject)
{
	CAutoLock lock(m_opFilter -> GetLock());
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

STDMETHODIMP EMWinDXPluginEngineInputPin::Receive(IMediaSample* pSample)
{
	if((! m_vBufferWaiting) || m_opStoredBuffer == NULL)
		return E_FAIL;

	BYTE* vSourceData = NULL;
	pSample -> GetPointer(&vSourceData);
	uint32 vBytesToCopy = pSample -> GetActualDataLength();
	BYTE* vTargetData = (BYTE*) m_opStoredBuffer -> Data();
	memcpy(vTargetData, vSourceData, vBytesToCopy);
	m_vBufferWaiting = false;
	return S_OK;
}

bool EMWinDXPluginEngineInputPin::SetTargetBuffer(EMMediaDataBuffer* p_opBuffer)
{
	//if(m_opStoredBuffer == NULL && (! m_vBufferWaiting))
	//{
		m_opStoredBuffer = p_opBuffer;
		m_vBufferWaiting = true;
	//}
	//else
	//	return false;
	return true;
}

bool EMWinDXPluginEngineInputPin::AssertBufferProcessed()
{
	if(m_opStoredBuffer != NULL && (! m_vBufferWaiting))
	{
		m_opStoredBuffer = NULL;
		m_vBufferWaiting = false;
		return true;
	}
	return false;

	//Set the target buffer that the producer should copy the IMediaSample data into...
//	m_opWaitingBuffer = p_opBuffer;
	
	//Release the producer/incoming thread since there's now a buffer ready to fill with stuff!
//	ReleaseB(); 
	
	//Make sure we don't go on until the producer has altered the contents for us!
//	AcquireA();

	//OK! Now we have an altered buffer - so return it and we're done!
}

HRESULT EMWinDXPluginEngineInputPin::Run(REFERENCE_TIME tStart)
{
	CAutoLock lock(m_opFilter -> GetLock());
	HRESULT hr = CRenderedInputPin::Run(tStart);
	if(hr != S_OK)
		return hr;
	return NOERROR;
}

HRESULT EMWinDXPluginEngineInputPin::Active()

{
	CAutoLock lock(m_opFilter -> GetLock());
	HRESULT h = CRenderedInputPin::Active();
	return NOERROR;
}

HRESULT EMWinDXPluginEngineInputPin::Inactive()
{
	CAutoLock lock(m_opFilter -> GetLock());
	HRESULT hr = CRenderedInputPin::Inactive();
	if(hr != S_OK)
		return hr;
	return NOERROR;
}

#endif